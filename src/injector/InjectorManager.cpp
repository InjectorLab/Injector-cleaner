#include "InjectorManager.h"

InjectorManager::InjectorManager(uint16_t defaultDelayMs, uint16_t defaultPulseMs)
: delayMs_(defaultDelayMs), pulseMs_(defaultPulseMs) {}

void InjectorManager::setup() {
    for (uint8_t i = 0; i < injectorCount_; ++i) {
        pinMode(injectorPins_[i], OUTPUT);
        digitalWrite(injectorPins_[i], LOW);
    }
}

void InjectorManager::loop() {
    if (!running_) {
        return;
    }

    const uint32_t now = millis();
    if (now < nextPhaseAtMs_) {
        return;
    }

    switch (phase_) {
        case PULSE_HIGH: {
            digitalWrite(activePin_, LOW);
            phase_ = GAP_DELAY;
            nextPhaseAtMs_ = now + delayMs_;
            break;
        }
        case GAP_DELAY:
        case IDLE:
        default: {
            uint8_t nextPin = 0;
            if (!readNextPinFromPattern_(nextPin)) {
                stopPattern();
                return;
            }
            activePin_ = nextPin;
            digitalWrite(activePin_, HIGH);
            phase_ = PULSE_HIGH;
            nextPhaseAtMs_ = now + pulseMs_;
            break;
        }
    }
}

void InjectorManager::addInjectorPin(uint8_t pin) {
    if (injectorCount_ < kMaxInjectors) {
        injectorPins_[injectorCount_++] = pin;
    }
}

void InjectorManager::setTimings(uint16_t delayMs, uint16_t pulseMs) {
    if (delayMs > 0) delayMs_ = delayMs;
    if (pulseMs > 0) pulseMs_ = pulseMs;
}

void InjectorManager::startPattern(const uint8_t* pattern, uint8_t length, long repeatCount) {
    patternLen_ = (length > kMaxPattern) ? kMaxPattern : length;
    for (uint8_t i = 0; i < patternLen_; ++i) pattern_[i] = pattern[i];

    repeatCount_ = repeatCount;
    repeatLeft_  = repeatCount;
    patternIndex_ = 0;
    phase_ = IDLE;
    running_ = true;
    nextPhaseAtMs_ = millis();
}

void InjectorManager::stopPattern() {
    setAllLow_();
    running_ = false;
    phase_ = IDLE;
}

bool InjectorManager::isRunning() const { return running_; }
uint16_t InjectorManager::getDelayMs() const { return delayMs_; }
uint16_t InjectorManager::getPulseMs() const { return pulseMs_; }
uint8_t  InjectorManager::getInjectorCount() const { return injectorCount_; }

void InjectorManager::setAllLow_() {
    for (uint8_t i = 0; i < injectorCount_; ++i) {
        digitalWrite(injectorPins_[i], LOW);
    }
}

bool InjectorManager::readNextPinFromPattern_(uint8_t& outPin) {
    if (patternLen_ == 0) {
        return false;
    }

    // Конец цикла по паттерну?
    if (patternIndex_ >= patternLen_) {
        patternIndex_ = 0;
        if (repeatCount_ > 0) {
            repeatLeft_--;
            if (repeatLeft_ <= 0) {
                return false;
            }
        }
    }

    const uint8_t logicalIndex = pattern_[patternIndex_++];
    if (logicalIndex == 0 || logicalIndex > injectorCount_) {
        return readNextPinFromPattern_(outPin);
    }
    outPin = injectorPins_[logicalIndex - 1];
    return true;
}