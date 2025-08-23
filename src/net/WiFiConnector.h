#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include "../common/LifeCycleHandler.h"

class WiFiConnector final : public LifeCycleHandler {
public:
    WiFiConnector(const char* ssid, const char* pass);

    void setup() override;
    void loop(uint32_t cycleStartMillis) override;

    bool isConnected() const;
    IPAddress getIp() const;

private:
    const char* ssid_;
    const char* pass_;
    bool hasIp_ = false;
    unsigned long nextReconnectAttemptMs_ = 0;

    void startConnectIfNeeded_();
    static void onWiFiEvent_(WiFiEvent_t event);
};