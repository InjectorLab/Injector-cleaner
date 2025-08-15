#pragma once
#include <Arduino.h>
#include "IInjectorAdapter.h"

class PinInjectorAdapter : public IInjectorAdapter {
public:
    explicit PinInjectorAdapter(uint8_t pin, bool activeHigh = true)
    : pin_(pin), activeHigh_(activeHigh) {}

    void setup() override {
        pinMode(pin_, OUTPUT);
        off();
    }

    void on() override {
        digitalWrite(pin_, activeHigh_ ? HIGH : LOW);
    }

    void off() override {
        digitalWrite(pin_, activeHigh_ ? LOW : HIGH);
    }

private:
    uint8_t pin_;
    bool activeHigh_;
};