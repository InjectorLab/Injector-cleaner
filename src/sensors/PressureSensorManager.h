#pragma once
#include <Arduino.h>
#include "../common/LifeCycleHandler.h"
#include "./adapters/IPressureSensorAdapter.h"

class PressureSensorManager final : public LifeCycleHandler {
public:
    explicit PressureSensorManager(IPressureSensorAdapter& adapter);

    void setup() override;
    void loop(uint32_t cycleStartMillis) override;

    uint16_t getCurrentPressureRaw() const;

private:
    IPressureSensorAdapter& adapter_;

    static constexpr uint8_t windowSize_ = 10;
    uint16_t ring_[windowSize_] = {0};
    uint8_t ringIndex_ = 0;
    uint32_t sum_ = 0;
    uint16_t current_ = 0;

    static constexpr uint32_t samplePeriodMs_ = 10;
    uint32_t nextSampleAtMs_ = 0;

    void sampleOnce_();
};