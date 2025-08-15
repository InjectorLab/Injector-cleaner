#pragma once
#include <Arduino.h>

class IInjectorAdapter {
public:
    virtual void setup() = 0;
    virtual void on() = 0;
    virtual void off() = 0;
    virtual ~IInjectorAdapter() = default;
};