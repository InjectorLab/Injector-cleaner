#pragma once
#include <Arduino.h>
#include "IPumpAdapter.h"
#include "../../lilygo/RelayBus.h"

class RelayBusPumpAdapter : public IPumpAdapter {
public:
    explicit RelayBusPumpAdapter(RelayBus& bus, uint8_t channel1to8, bool activeHigh = true)
    : bus_(bus), ch_(channel1to8), activeHigh_(activeHigh) {}

    void setup() override {
        off();
    }

    void setPowerPercent(uint8_t percent) override {
        const bool wantOn = (percent > 0);
        const bool busOn = activeHigh_ ? wantOn : !wantOn;
        bus_.setChannel(ch_, busOn);
        lastOn_ = wantOn;
    }

    void off() override {
        const bool busOn = activeHigh_ ? false : true;
        bus_.setChannel(ch_, busOn);
        lastOn_ = false;
    }

    bool isOn() const { return lastOn_; }

private:
    RelayBus& bus_;
    uint8_t   ch_;
    bool      activeHigh_;
    bool      lastOn_ = false;
};