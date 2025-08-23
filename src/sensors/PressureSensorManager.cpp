#include "PressureSensorManager.h"

PressureSensorManager::PressureSensorManager(IPressureSensorAdapter& adapter)
: adapter_(adapter) {}

void PressureSensorManager::setup() {
    adapter_.setup();

    uint16_t firstValue = static_cast<uint16_t>(adapter_.readRaw());
    for (uint8_t i = 0; i < windowSize_; ++i) {
        ring_[i] = firstValue;
        sum_ += firstValue;
    }
    current_ = firstValue;
}

void PressureSensorManager::loop(uint32_t cycleStartMillis) {
    if (cycleStartMillis >= nextSampleAtMs_) {
        nextSampleAtMs_ = cycleStartMillis + samplePeriodMs_;
        sampleOnce_();
    }
}

uint16_t PressureSensorManager::getCurrentPressureRaw() const {
    return current_;
}

void PressureSensorManager::sampleOnce_() {
    const uint16_t raw = adapter_.readRaw();
    sum_ -= ring_[ringIndex_];
    ring_[ringIndex_] = raw;
    sum_ += raw;
    ringIndex_ = (ringIndex_ + 1) % windowSize_;
    current_ = static_cast<uint16_t>(sum_ / windowSize_);
}