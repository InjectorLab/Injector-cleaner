#pragma once
#include <Arduino.h>
#include "IPressureSensorAdapter.h"

// Реализация через прямое чтение ADC с указанного пина
class PinAdcPressureSensorAdapter : public IPressureSensorAdapter {
public:
    explicit PinAdcPressureSensorAdapter(uint8_t adcPin) : adcPin_(adcPin) {}

    void setup() override {
        pinMode(adcPin_, INPUT);
    }

    uint16_t readRaw() override {
        return static_cast<uint16_t>(analogRead(adcPin_));
    }

private:
    uint8_t adcPin_;
};