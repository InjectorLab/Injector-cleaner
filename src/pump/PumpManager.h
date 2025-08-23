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
    void loop() override;

    void applySettings(bool autoEnable, int cutoffRaw, int maxPower = 100);
    
    bool isEnabled() const;
    bool isPumpOn() const;

    int  getCutoffPressureRaw() const;

    uint8_t  getMaxPower() const;
    uint8_t  getCurrentPower() const;

private:
    static constexpr uint8_t minPower = 10;

    IPumpAdapter& pumpAdapter_;
    PressureSensorManager& pressureSensor_;

    bool enable_ = false;
    bool outputOn_ = false;
    int  cutoffRaw_   = 3800;
    int  hysteresisRaw_ = 100;
    int  controlRangeRaw_  = 800;

    uint8_t  maxPower_      = 100;
    uint8_t  currentPower_ = minPower;

    void writePower_(uint8_t percent);
    void writeOff_();
    void evaluateAutoControl_();
};