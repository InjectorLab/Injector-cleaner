#pragma once
#include <Arduino.h>
#include "../common/LifeCycleHandler.h"

class InjectorManager final : public LifeCycleHandler {
public:
    static constexpr uint8_t kMaxInjectors = 8;
    static constexpr uint8_t kMaxPattern   = 32;

    InjectorManager(uint16_t defaultDelayMs, uint16_t defaultPulseMs);

    void setup() override;
    void loop() override;

    void addInjectorPin(uint8_t pin);

    void setTimings(uint16_t delayMs, uint16_t pulseMs);
    void startPattern(const uint8_t* pattern, uint8_t length, long repeatCount);
    void stopPattern();

    bool     isRunning() const;
    uint16_t getDelayMs() const;
    uint16_t getPulseMs() const;

    uint8_t  getInjectorCount() const;
private:
    uint8_t injectorPins_[kMaxInjectors] = {0};
    uint8_t injectorCount_ = 0;

    uint16_t delayMs_;
    uint16_t pulseMs_;

    bool running_ = false;

    enum Phase { IDLE, PULSE_HIGH, GAP_DELAY } phase_ = IDLE;
    uint8_t  pattern_[kMaxPattern] = {0};
    uint8_t  patternLen_ = 0;
    long     repeatCount_ = 0;
    long     repeatLeft_ = 0;
    uint8_t  patternIndex_ = 0;
    uint8_t  activePin_ = 0;
    uint32_t nextPhaseAtMs_ = 0;

    void setAllLow_();
    bool readNextPinFromPattern_(uint8_t& outPin);
};