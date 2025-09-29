#pragma once
#include <Arduino.h>
#include "../common/LifeCycleHandler.h"
#include "../common/StateVersionTracker.h"
#include "../injector/InjectorManager.h"
#include "../pump/PumpManager.h"

class TimerManager final : public LifeCycleHandler, public StateVersionTracker  {
public:
    explicit TimerManager(InjectorManager& injector, PumpManager& pump);

    void setup() override;
    void loop(uint32_t cycleStartMillis) override;

    void start(uint32_t durationMs);
    void stop();

    bool     isRunning() const;
    uint32_t getRemainingMs() const;
private:
    InjectorManager& injector_;
    PumpManager& pump_;

    bool running_ = false;
    uint32_t startedAtMs_ = 0;
    uint32_t durationMs_  = 0;

    void startInjectorsInfinite_();
    void stopInjectors_();
};