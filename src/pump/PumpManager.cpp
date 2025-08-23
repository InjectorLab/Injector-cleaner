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

void PumpManager::applySettings(bool enable, int cutoffRaw, int maxPower) {
    if (enable_ != enable) {
        enable_ = enable;
    }
    if (cutoffRaw < 0) {
        cutoffRaw = 1;
    }
    cutoffRaw_ = cutoffRaw;

    if (maxPower >= 0) {
        if (maxPower > 100){
            maxPower = 100;
        }
        if (maxPower < 0) {
            maxPower = 0;
        }
        maxPower_ = static_cast<uint8_t>(maxPower);
    }
    bumpVersion();
}

bool PumpManager::isEnabled() const { return enable_; }
int  PumpManager::getCutoffPressureRaw() const { return cutoffRaw_; }
bool PumpManager::isPumpOn() const { return outputOn_; }
uint8_t PumpManager::getMaxPower() const { return maxPower_; }
uint8_t PumpManager::getCurrentPower() const { return currentPower_; }

void PumpManager::writePower_(uint8_t percent) {
    if (percent > maxPower_) {
        percent = maxPower_;
    }

    if (percent < minPower) {
        percent = minPower;
    }

    pumpAdapter_.setPowerPercent(percent);
    currentPower_ = percent;
    outputOn_ = percent > 0;
}

void PumpManager::writeOff_() {
    pumpAdapter_.off();
    if (outputOn_ || currentPower_ > 0) {
        outputOn_ = false;
        currentPower_ = 0;
    }
}

void PumpManager::evaluateAutoControl_() {
    if (!enable_) {
        writeOff_();
        return;
    }

    const uint16_t pressure = pressureSensor_.getCurrentPressureRaw();

    if (pressure >= cutoffRaw_) {
        writeOff_();
        return;
    }

    const int32_t difference = static_cast<int32_t>(cutoffRaw_) - static_cast<int32_t>(pressure);

    if (difference <= hysteresisRaw_) {
        writeOff_();
        return;
    }

    int32_t errBeyond = difference - hysteresisRaw_;
    int32_t range     = controlRangeRaw_;
    if (range < 1) {
        range = 1;
    }

    int32_t target = (static_cast<int32_t>(maxPower_) * errBeyond) / range;

    if (target > maxPower_) {
        target = maxPower_;
    }
    if (target < 0) {
        target = 0;
    }

    if (target == 0) {
        writeOff_();
        return;
    }
    
    if (target < minPower) {
        target = minPower;
    }

    writePower_(static_cast<uint8_t>(target));
}