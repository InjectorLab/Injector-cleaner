#pragma once
#include <Arduino.h>

class IPressureSensorAdapter {
public:
    virtual void setup() = 0;
    virtual uint16_t readRaw() = 0;
    virtual ~IPressureSensorAdapter() = default;
};