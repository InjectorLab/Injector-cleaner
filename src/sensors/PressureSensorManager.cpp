#include "PressureSensorManager.h"

PressureSensorManager::PressureSensorManager(uint8_t adcPin)
: adcPin_(adcPin) {}

void PressureSensorManager::setup() {
    pinMode(adcPin_, INPUT);

    uint16_t firstValue = static_cast<uint16_t>(analogRead(adcPin_));
    for (uint8_t i = 0; i < windowSize_; ++i) {
        ring_[i] = firstValue;
        sum_ += firstValue;
    }
    current_ = firstValue;
}

void PressureSensorManager::loop() {
    const uint32_t now = millis();
    if (now >= nextSampleAtMs_) {
        nextSampleAtMs_ = now + samplePeriodMs_;
        sampleOnce_();
    }
}

uint16_t PressureSensorManager::getCurrentPressureRaw() const {
    return current_;
}

void PressureSensorManager::sampleOnce_() {
    const uint16_t raw = static_cast<uint16_t>(analogRead(adcPin_));
    sum_ -= ring_[ringIndex_];
    ring_[ringIndex_] = raw;
    sum_ += raw;
    ringIndex_ = (ringIndex_ + 1) % windowSize_;
    current_ = static_cast<uint16_t>(sum_ / windowSize_);
}