#include "LedcPumpAdapter.h"

LedcPumpAdapter::LedcPumpAdapter(uint8_t pin, bool activeHigh)
: pin_(pin), activeHigh_(activeHigh), lastPercent_(0) {}

void LedcPumpAdapter::setup() {
    ledcSetup(CHANNEL, FREQ_HZ, RESOLUTION);
    ledcAttachPin(pin_, CHANNEL);
    setPowerPercent(0);
}

void LedcPumpAdapter::setPowerPercent(uint8_t percent) {
    if (percent > 100) {
        percent = 100;
    }
    if (percent < 0) {
        percent = 0;
    }

    uint32_t duty = (percent * ((1 << RESOLUTION) - 1)) / 100;
    ledcWrite(CHANNEL, duty);
    lastPercent_ = percent;
}

void LedcPumpAdapter::off() {
    setPowerPercent(0);
}

uint8_t LedcPumpAdapter::getLastPercent() const {
    return lastPercent_;
}