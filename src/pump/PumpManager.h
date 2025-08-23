#pragma once
#include <Arduino.h>
#include "../common/LifeCycleHandler.h"
#include "../common/StateVersionTracker.h"
#include "../sensors/PressureSensorManager.h"
#include "./adapters/IPumpAdapter.h"

class PumpManager final : public LifeCycleHandler, public StateVersionTracker {
public:
    PumpManager(IPumpAdapter& pumpAdapter, PressureSensorManager& pressureSensor);

    void setup() override;
    void loop(uint32_t cycleStartMillis) override;

    void applySettings(bool autoEnable, int cutoffRaw, int maxPower = 100);
    
    bool isEnabled() const;
    bool isPumpOn() const;

    int  getCutoffPressureRaw() const;

    int  getMaxPower() const;
    int  getCurrentPower() const;

private:
    static constexpr int minPower = 10;

    IPumpAdapter& pumpAdapter_;
    PressureSensorManager& pressureSensor_;

    bool enable_ = false;
    bool outputOn_ = false;
    int  cutoffRaw_   = 3800;
    int  hysteresisRaw_ = 100;
    int  controlRangeRaw_  = 800;

    int  maxPower_      = 100;
    int  currentPower_ = minPower;

    void writePower_(int percent);
    void writeOff_();
    void evaluateAutoControl_();
};