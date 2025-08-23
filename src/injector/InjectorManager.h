#pragma once
#include <Arduino.h>
#include "../common/LifeCycleHandler.h"
#include "../common/StateVersionTracker.h"
#include "./adapters/IInjectorAdapter.h"

class InjectorManager final : public LifeCycleHandler, public StateVersionTracker {
public:
    static constexpr uint8_t kMaxInjectors = 8;
    static constexpr uint8_t kMaxPattern   = 32;

    InjectorManager(uint16_t defaultDelayMs, uint16_t defaultPulseMs);

    void setup() override;
    void loop(uint32_t cycleStartMillis) override;

    void addInjector(IInjectorAdapter& adapter);

    void setTimings(uint16_t delayMs, uint16_t pulseMs);
    void startPattern(const uint8_t* pattern, uint8_t length, long repeatCount);
    void stopPattern();

    bool     isRunning() const;
    uint16_t getDelayMs() const;
    uint16_t getPulseMs() const;

    uint8_t  getInjectorCount() const;
private:
    IInjectorAdapter* injectors_[kMaxInjectors] = {nullptr};
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
    int8_t   activeIndex_ = -1;
    uint32_t nextPhaseAtMs_ = 0;

    void setAllOff_();
    bool readNextFromPattern_(uint8_t& outIndex);
};