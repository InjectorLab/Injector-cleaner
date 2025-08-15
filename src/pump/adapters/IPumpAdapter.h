#pragma once
#include <Arduino.h>

class IPumpAdapter {
public:
    virtual void setup() = 0;
    virtual void setPowerPercent(uint8_t percent) = 0;
    virtual void off() = 0;
    virtual ~IPumpAdapter() = default;
};