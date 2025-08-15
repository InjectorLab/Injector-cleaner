#pragma once
#include <Arduino.h>
#include "IPumpAdapter.h"

class PinRelayPumpAdapter : public IPumpAdapter {
public:
    explicit PinRelayPumpAdapter(uint8_t pin, bool activeHigh = true)
    : pin_(pin), activeHigh_(activeHigh) {}

    void setup() override {
        pinMode(pin_, OUTPUT);
        write_(false);
    }

    void setPowerPercent(uint8_t percent) override {
        write_(percent > 0);
    }

    void off() override {
        write_(false);
    }

    bool isOn() const { return lastOn_; }

private:
    uint8_t pin_;
    bool activeHigh_;
    bool lastOn_ = false;

    void write_(bool on) {
        lastOn_ = on;
        const uint8_t level = activeHigh_ ? (on ? HIGH : LOW)
                                          : (on ? LOW  : HIGH);
        digitalWrite(pin_, level);
    }
};