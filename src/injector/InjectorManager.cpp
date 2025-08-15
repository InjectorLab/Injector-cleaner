#include "InjectorManager.h"

InjectorManager::InjectorManager(uint16_t defaultDelayMs, uint16_t defaultPulseMs)
: delayMs_(defaultDelayMs), pulseMs_(defaultPulseMs) {}

void InjectorManager::setup() {
    for (uint8_t i = 0; i < injectorCount_; ++i) {
        if (injectors_[i]) injectors_[i]->setup();
    }
    setAllOff_();
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
            if (activeIndex_ >= 0 && activeIndex_ < (int8_t)injectorCount_ && injectors_[activeIndex_]) {
                injectors_[activeIndex_]->off();
            }
            phase_ = GAP_DELAY;
            nextPhaseAtMs_ = now + delayMs_;
            break;
        }
        case GAP_DELAY:
        case IDLE:
        default: {
            uint8_t nextIdx = 0;
            if (!readNextFromPattern_(nextIdx)) {
                stopPattern();
                return;
            }
            activeIndex_ = nextIdx;
            if (injectors_[activeIndex_]) {
                injectors_[activeIndex_]->on();
            }
            phase_ = PULSE_HIGH;
            nextPhaseAtMs_ = now + pulseMs_;
            break;
        }
    }
}

void InjectorManager::addInjector(IInjectorAdapter& adapter) {
    if (injectorCount_ < kMaxInjectors) {
        injectors_[injectorCount_++] = &adapter;
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
    setAllOff_();
    running_ = false;
    phase_ = IDLE;
    activeIndex_ = -1;
}

bool InjectorManager::isRunning() const { return running_; }
uint16_t InjectorManager::getDelayMs() const { return delayMs_; }
uint16_t InjectorManager::getPulseMs() const { return pulseMs_; }
uint8_t  InjectorManager::getInjectorCount() const { return injectorCount_; }

void InjectorManager::setAllOff_() {
    for (uint8_t i = 0; i < injectorCount_; ++i) {
        if (injectors_[i]) injectors_[i]->off();
    }
}

bool InjectorManager::readNextFromPattern_(uint8_t& outIndex) {
    if (patternLen_ == 0 || injectorCount_ == 0) {
        return false;
    }

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
        return readNextFromPattern_(outIndex);
    }

    outIndex = logicalIndex - 1;
    return true;
}