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

    void applySettings(bool autoEnable, int cutoffRaw);
    bool isEnabled() const;
    int  getCutoffPressureRaw() const;
    bool isPumpOn() const;

private:
    IPumpAdapter& pumpAdapter_;
    PressureSensorManager& pressureSensor_;

    bool enable_ = false;
    bool outputOn_ = false;
    int  cutoffRaw_   = 3800;
    int  hysteresisRaw_ = 100;

    void writePower_(uint8_t percent);
    void writeOff_();
    void evaluateAutoControl_();
};