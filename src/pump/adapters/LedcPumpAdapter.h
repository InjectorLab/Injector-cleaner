#pragma once
#include <Arduino.h>
#include "IPumpAdapter.h"

class LedcPumpAdapter : public IPumpAdapter {
public:
    explicit LedcPumpAdapter(uint8_t pin, bool activeHigh = true);

    void setup() override;
    void setPowerPercent(uint8_t percent) override;
    void off() override;

    uint8_t getLastPercent() const;

private:
    uint8_t pin_;
    bool activeHigh_;
    uint8_t lastPercent_;

    static constexpr uint8_t CHANNEL    = 0;
    static constexpr uint16_t FREQ_HZ   = 20;  // 20 Hz is required by J538 as control input
    static constexpr uint8_t RESOLUTION = 8;
};