#pragma once
#include <Arduino.h>
#include <ArduinoOTA.h>
#include "../common/LifeCycleHandler.h"

class OtaManager final : public LifeCycleHandler {
public:
    explicit OtaManager(const char* hostname = "injector-cleaner")
    : hostname_(hostname) {}

    void setup() override;
    void loop(uint32_t cycleStartMillis) override;
private:
    const char*          hostname_ = nullptr;
};