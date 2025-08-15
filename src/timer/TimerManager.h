#pragma once
#include <Arduino.h>
#include "../common/LifeCycleHandler.h"
#include "../injector/InjectorManager.h"

class TimerManager final : public LifeCycleHandler {
public:
    explicit TimerManager(InjectorManager& injector);

    void setup() override;
    void loop() override;

    void start(uint32_t durationMs);
    void stop();

    bool     isRunning() const;
    uint32_t getRemainingMs() const;
private:
    InjectorManager& injector_;

    bool running_ = false;
    uint32_t startedAtMs_ = 0;
    uint32_t durationMs_  = 0;

    void startInjectorsInfinite_();
    void stopInjectors_();
};