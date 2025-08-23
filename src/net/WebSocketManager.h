#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "../common/LifeCycleHandler.h"
#include "../sensors/PressureSensorManager.h"
#include "../pump/PumpManager.h"
#include "../injector/InjectorManager.h"
#include "../timer/TimerManager.h"

class WebSocketManager final : public LifeCycleHandler {
public:
    WebSocketManager(PressureSensorManager& pressure,
                     PumpManager& pump,
                     InjectorManager& injector,
                     TimerManager& timer);

    void setup() override;
    void loop(uint32_t cycleStartMillis) override;

private:
    AsyncWebServer httpServer_{80};
    AsyncWebSocket ws_{"/ws"};

    PressureSensorManager& pressure_;
    PumpManager& pump_;
    InjectorManager& injector_;
    TimerManager& timer_;

    static constexpr uint32_t pumpTelemetryPeriodMs_  = 1000;
    static constexpr uint32_t timerTelemetryPeriodMs_ = 1000;

    uint32_t nextPumpTelemetryAtMs_  = 0;
    uint32_t nextTimerTelemetryAtMs_ = 0;
    uint32_t lastTiemrRev_ = 0;
    uint32_t lastInjectorRev_ = 0;

    void onWsEvent_(AsyncWebSocket* server, AsyncWebSocketClient* client,
                    AwsEventType type, void* arg, uint8_t* data, size_t len);

    void handleCommandJson_(AsyncWebSocketClient* client, const char* json, size_t len);

    void sendPumpStatusTo_(AsyncWebSocketClient* client);
    void broadcastPumpStatus_();

    void sendInjectorStatusTo_(AsyncWebSocketClient* client);
    void broadcastInjectorStatus_();

    void sendTimerStatusTo_(AsyncWebSocketClient* client);
    void broadcastTimerStatus_();

    void sendPong_(AsyncWebSocketClient* client, JsonVariantConst pingId);
};