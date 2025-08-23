#pragma once
#include <Arduino.h>
#include "../common/LifeCycleHandler.h"

class MonitoringManager final : public LifeCycleHandler {
public:
    explicit MonitoringManager(uint32_t intervalMs = 5000)
    : intervalMs_(intervalMs) {}

    void setup() override;
    void loop(uint32_t cycleStartMillis)  override;

    void logNow();
private:
    void logOnce_();
    uint32_t    intervalMs_;
    uint32_t    lastLogMs_  = 0;

    size_t      startFreeHeap_  = 0;
    size_t      startFreePsram_ = 0;
};