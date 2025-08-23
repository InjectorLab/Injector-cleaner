#include "OpenDrainPumpAdapter.h"

OpenDrainPumpAdapter::OpenDrainPumpAdapter(int pin)
: pin_(pin), dutyPercent_(0), lastToggleMs_(0), stateLow_(false) {}

void OpenDrainPumpAdapter::setup() {
    pinMode(pin_, OUTPUT_OPEN_DRAIN); 
    digitalWrite(pin_, HIGH);
    dutyPercent_ = 0;
    lastToggleMs_ = millis();
    stateLow_ = false;
}

void OpenDrainPumpAdapter::setPowerPercent(uint8_t percent) {
    if (percent > 100) {
        percent = 100;
    }
    dutyPercent_ = percent;
}

void OpenDrainPumpAdapter::off() {
    dutyPercent_ = 0;
    digitalWrite(pin_, HIGH);
    stateLow_ = false;
}

void OpenDrainPumpAdapter::loop() {
    if (dutyPercent_ == 0) {
        digitalWrite(pin_, HIGH);
        stateLow_ = false;
        return;
    }
    if (dutyPercent_ == 100) {
        digitalWrite(pin_, LOW);
        stateLow_ = true;
        return;
    }

    int now = millis();
    int elapsed = now - lastToggleMs_;

    if (elapsed >= PERIOD_MS) {
        lastToggleMs_ = now;
        stateLow_ = false;
    }

    int onTime = (PERIOD_MS * dutyPercent_) / 100;

    if (elapsed < onTime) {
        digitalWrite(pin_, LOW);
        stateLow_ = true;
    } else {
        digitalWrite(pin_, HIGH);
        stateLow_ = false;
    }
}