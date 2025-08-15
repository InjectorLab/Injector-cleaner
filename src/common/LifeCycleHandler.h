#pragma once
#include <Arduino.h>

class LifeCycleHandler {
public:
    virtual void setup() = 0;
    virtual void loop() = 0;
    virtual ~LifeCycleHandler() = default;
};