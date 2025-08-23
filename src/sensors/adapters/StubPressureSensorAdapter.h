#pragma once
#include <Arduino.h>
#include "IPressureSensorAdapter.h"

class StubPressureSensorAdapter : public IPressureSensorAdapter {
public:
    explicit StubPressureSensorAdapter(uint16_t value);

    void setup() override;
    uint16_t readRaw() override;

private:
    uint16_t value_;
};