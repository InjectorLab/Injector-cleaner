#include "PumpManager.h"

PumpManager::PumpManager(IPumpAdapter& pumpAdapter, PressureSensorManager& pressureSensor)
: pumpAdapter_(pumpAdapter), pressureSensor_(pressureSensor) {}

void PumpManager::setup() {
    pumpAdapter_.setup();
    writeOff_();
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
    bumpVersion();
}

bool PumpManager::isEnabled() const { return enable_; }
int  PumpManager::getCutoffPressureRaw() const { return cutoffRaw_; }
bool PumpManager::isPumpOn() const { return outputOn_; }

void PumpManager::writePower_(uint8_t percent) {
    pumpAdapter_.setPowerPercent(percent);
    const bool newOn = (percent > 0);
    if (newOn != outputOn_) {
        outputOn_ = newOn;
    }
}

void PumpManager::writeOff_() {
    pumpAdapter_.off();
    if (outputOn_) {
        outputOn_ = false;
    }
}

void PumpManager::evaluateAutoControl_() {
    const uint16_t pressure = pressureSensor_.getCurrentPressureRaw();

    if (!enable_) {
        writeOff_();
        return;
    }

    if (pressure >= cutoffRaw_) {
        writeOff_();
    } else if (pressure < (cutoffRaw_ - hysteresisRaw_)) {
        writePower_(100);
    }
}