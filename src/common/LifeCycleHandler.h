#pragma once
#include <Arduino.h>

class LifeCycleHandler {
public:
    virtual void setup() = 0;
    virtual void loop(uint32_t cycleStartMillis) = 0;
    virtual ~LifeCycleHandler() = default;
};