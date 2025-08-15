#include "PumpManager.h"

PumpManager::PumpManager(uint8_t relayPin, PressureSensorManager& pressureSensor)
: relayPin_(relayPin), pressureSensor_(pressureSensor) {}

void PumpManager::setup() {
    pinMode(relayPin_, OUTPUT);
    writeRelay_(false);
}

void PumpManager::loop() {
    evaluateAutoControl_();
}

void PumpManager::applySettings(bool enable, int cutoffRaw) {
    if (enable_ != enable) {
        enable_ = enable;
    }
    if (cutoffRaw < 0) {
        cutoffRaw = 1;
    }
    cutoffRaw_ = cutoffRaw;
}

bool PumpManager::isEnabled() const { return enable_; }
int  PumpManager::getCutoffPressureRaw() const { return cutoffRaw_; }
bool PumpManager::isRelayOn() const { return relayOn_; }

void PumpManager::writeRelay_(bool on) {
    digitalWrite(relayPin_, on ? HIGH : LOW);
    relayOn_ = on;
}

void PumpManager::evaluateAutoControl_() {
    const uint16_t pressure = pressureSensor_.getCurrentPressureRaw();

    bool newRelay = relayOn_;
    if (!enable_) {
        newRelay = false;
    } else {
        if (pressure >= cutoffRaw_) {
            newRelay = false;
        } else if (pressure < (cutoffRaw_ - hysteresisRaw_)) {
            newRelay = true;
        }
    }

    if (newRelay != relayOn_) {
        writeRelay_(newRelay);
    }
}