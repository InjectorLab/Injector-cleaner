#include "WebSocketManager.h"

WebSocketManager::WebSocketManager(PressureSensorManager& pressure,
                                   PumpManager& pump,
                                   InjectorManager& injector,
                                   TimerManager& timer)
: pressure_(pressure), pump_(pump), injector_(injector), timer_(timer) {}

void WebSocketManager::setup() {
    httpServer_.on("/", HTTP_GET, [](AsyncWebServerRequest* req){
        req->send(200, "text/plain", "OK");
    });

    ws_.onEvent([this](AsyncWebSocket* s, AsyncWebSocketClient* c, AwsEventType t, void* a, uint8_t* d, size_t l){
        this->onWsEvent_(s, c, t, a, d, l);
    });

    httpServer_.addHandler(&ws_);
    httpServer_.begin();

    Serial.println("[HTTP] Server started on :80, WS at /ws");
}

void WebSocketManager::loop() {
    const uint32_t now = millis();

    if (now >= nextPumpTelemetryAtMs_) {
        nextPumpTelemetryAtMs_ = now + pumpTelemetryPeriodMs_;
        broadcastPumpStatus_();
    }

    if (timer_.isRunning() && now >= nextTimerTelemetryAtMs_) {
        nextTimerTelemetryAtMs_ = now + timerTelemetryPeriodMs_;
        broadcastTimerStatus_();
    }
}

void WebSocketManager::onWsEvent_(AsyncWebSocket* server, AsyncWebSocketClient* client,
                                  AwsEventType type, void* arg, uint8_t* data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.printf("[WS] Client #%u connected\n", client->id());
        sendPumpStatusTo_(client);
        sendInjectorStatusTo_(client);
        sendTimerStatusTo_(client);
        return;
    }

    if (type == WS_EVT_DATA) {
        auto* info = reinterpret_cast<AwsFrameInfo*>(arg);
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
            handleCommandJson_(client, reinterpret_cast<const char*>(data), len);
        }
        return;
    }

    if (type == WS_EVT_DISCONNECT) {
        Serial.printf("[WS] Client #%u disconnected\n", client->id());
        return;
    }
}

void WebSocketManager::handleCommandJson_(AsyncWebSocketClient* client, const char* json, size_t len) {
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, json, len);
    if (err) {
        Serial.println("[WS] JSON parse error");
        return;
    }

    const char* type = doc["type"] | "";
    if (strcmp(type, "system.ping") == 0) {
        sendPong_(client, doc["id"]);
        return;
    }

    if (strcmp(type, "pump.set") == 0) {
        const bool on = doc["on"] | true;
        const int  cutoff = doc["cutoff"] | pump_.getCutoffPressureRaw();
        pump_.applySettings(on, cutoff);
        broadcastPumpStatus_();
        return;
    }

    if (strcmp(type, "inj.set") == 0) {
        const uint16_t delayMs = doc["delay"] | injector_.getDelayMs();
        const uint16_t pulseMs = doc["pulse"] | injector_.getPulseMs();
        injector_.setTimings(delayMs, pulseMs);
        broadcastInjectorStatus_();
        return;
    }

    if (strcmp(type, "inj.open") == 0) {
        uint8_t pattern[InjectorManager::kMaxPattern];
        uint8_t n = 0;
        for (JsonVariant v : doc["pattern"].as<JsonArray>()) {
            if (n < InjectorManager::kMaxPattern) pattern[n++] = (uint8_t)v.as<uint16_t>();
        }
        long repeat = doc["repeat"] | 0L;
        injector_.startPattern(pattern, n, repeat);
        broadcastInjectorStatus_();
        return;
    }

    if (strcmp(type, "inj.stop") == 0) {
        injector_.stopPattern();
        broadcastInjectorStatus_();
        return;
    }

    if (strcmp(type, "timer.start") == 0) {
        const uint32_t durationMs = doc["time"] | 0u;
        timer_.start(durationMs);
        broadcastTimerStatus_();
        broadcastInjectorStatus_();
        return;
    }

    if (strcmp(type, "timer.stop") == 0) {
        timer_.stop();
        broadcastTimerStatus_();
        broadcastInjectorStatus_();
        return;
    }

    Serial.printf("[WS] Unknown type: %s\n", type);
}

void WebSocketManager::sendPumpStatusTo_(AsyncWebSocketClient* client) {
    JsonDocument doc;
    doc["type"]     = "pump.status";
    doc["on"]       = pump_.isRelayOn();
    doc["cutoff"]   = pump_.getCutoffPressureRaw();
    doc["pressure"] = pressure_.getCurrentPressureRaw();

    String out;
    serializeJson(doc, out);
    client->text(out);
}

void WebSocketManager::broadcastPumpStatus_() {
    JsonDocument doc;
    doc["type"]     = "pump.status";
    doc["on"]       = pump_.isRelayOn();
    doc["cutoff"]   = pump_.getCutoffPressureRaw();
    doc["pressure"] = pressure_.getCurrentPressureRaw();

    String out;
    serializeJson(doc, out);
    ws_.textAll(out);
}

void WebSocketManager::sendInjectorStatusTo_(AsyncWebSocketClient* client) {
    JsonDocument doc;
    doc["type"]    = "inj.status";
    doc["running"] = injector_.isRunning();
    doc["delay"]   = injector_.getDelayMs();
    doc["pulse"]   = injector_.getPulseMs();

    String out;
    serializeJson(doc, out);
    client->text(out);
}

void WebSocketManager::broadcastInjectorStatus_() {
    JsonDocument doc;
    doc["type"]    = "inj.status";
    doc["running"] = injector_.isRunning();
    doc["delay"]   = injector_.getDelayMs();
    doc["pulse"]   = injector_.getPulseMs();

    String out;
    serializeJson(doc, out);
    ws_.textAll(out);
}

void WebSocketManager::sendTimerStatusTo_(AsyncWebSocketClient* client) {
    JsonDocument doc;
    doc["type"]      = "timer.status";
    doc["running"]   = timer_.isRunning();
    doc["remaining"] = timer_.getRemainingMs();

    String out;
    serializeJson(doc, out);
    client->text(out);
}

void WebSocketManager::broadcastTimerStatus_() {
    JsonDocument doc;
    doc["type"]      = "timer.status";
    doc["running"]   = timer_.isRunning();
    doc["remaining"] = timer_.getRemainingMs();

    String out;
    serializeJson(doc, out);
    ws_.textAll(out);
}

void WebSocketManager::sendPong_(AsyncWebSocketClient* client, JsonVariantConst pingId) {
    JsonDocument pong;
    pong["type"] = "system.pong";
    if (!pingId.isNull()) pong["id"] = pingId;

    String out;
    serializeJson(pong, out);
    client->text(out);
}
