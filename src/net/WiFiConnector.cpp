#include "WiFiConnector.h"

WiFiConnector::WiFiConnector(const char* ssid, const char* pass)
: ssid_(ssid), pass_(pass) {}

void WiFiConnector::setup() {
    WiFi.mode(WIFI_STA);
    WiFi.onEvent(onWiFiEvent_);
    startConnectIfNeeded_();
}

void WiFiConnector::loop(uint32_t cycleStartMillis) {
    if (!isConnected() && cycleStartMillis >= nextReconnectAttemptMs_) {
        startConnectIfNeeded_();
    }
}

bool WiFiConnector::isConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

IPAddress WiFiConnector::getIp() const {
    return WiFi.localIP();
}

void WiFiConnector::startConnectIfNeeded_() {
    if (WiFi.status() == WL_CONNECTED) return;
    nextReconnectAttemptMs_ = millis() + 5000;
    WiFi.begin(ssid_, pass_);
}

void WiFiConnector::onWiFiEvent_(WiFiEvent_t event) {
    switch (event) {
        case SYSTEM_EVENT_STA_GOT_IP:
            Serial.printf("[WiFi] Connected, IP: %s\n", WiFi.localIP().toString().c_str());
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("[WiFi] Disconnected, retrying...");
            break;
        default:
            break;
    }
}