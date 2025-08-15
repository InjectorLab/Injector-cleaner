#pragma once
#include <Arduino.h>
#include "../common/LifeCycleHandler.h"

class PressureSensorManager final : public LifeCycleHandler {
public:
    explicit PressureSensorManager(uint8_t adcPin);

    void setup() override;
    void loop() override;

    uint16_t getCurrentPressureRaw() const;

private:
    const uint8_t adcPin_;
    static constexpr uint8_t windowSize_ = 10;
    uint16_t ring_[windowSize_] = {0};
    uint8_t ringIndex_ = 0;
    uint32_t sum_ = 0;
    uint16_t current_ = 0;

    static constexpr uint32_t samplePeriodMs_ = 10;
    uint32_t nextSampleAtMs_ = 0;

    void sampleOnce_();
};