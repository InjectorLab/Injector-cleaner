#pragma once
#include <Arduino.h>
#include "IPumpAdapter.h"

class OpenDrainPumpAdapter : public IPumpAdapter {
public:
    explicit OpenDrainPumpAdapter(int pin);

    void setup() override;
    void setPowerPercent(uint8_t percent) override;
    void off() override;

    void loop();

private:
    int pin_;
    int dutyPercent_;
    int lastToggleMs_;
    bool stateLow_;

    static constexpr int FREQ_HZ   = 20;              // J538 expects 20 Hz
    static constexpr int PERIOD_MS = 1000 / FREQ_HZ;  // 50 ms
};