#pragma once
#include <Arduino.h>
#include "IInjectorAdapter.h"
#include "../../lilygo/RelayBus.h"

class RelayBusInjectorAdapter : public IInjectorAdapter {
public:
    explicit RelayBusInjectorAdapter(RelayBus& bus, uint8_t channel1to8, bool activeHigh = true)
    : bus_(bus), ch_(channel1to8), activeHigh_(activeHigh) {}

    void setup() override {
        off();
    }

    void on() override {
        const bool busOn = activeHigh_ ? true : false;
        bus_.setChannel(ch_, busOn);
    }

    void off() override {
        const bool busOn = activeHigh_ ? false : true;
        bus_.setChannel(ch_, busOn);
    }

private:
    RelayBus& bus_;
    uint8_t   ch_;
    bool      activeHigh_;
};