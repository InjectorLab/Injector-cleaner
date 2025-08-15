#pragma once
#include <Arduino.h>
#include "../common/LifeCycleHandler.h"
#include "../sensors/PressureSensorManager.h"

class PumpManager final : public LifeCycleHandler {
public:
    PumpManager(uint8_t relayPin, PressureSensorManager& pressureSensor);

    void setup() override;
    void loop() override;

    void applySettings(bool autoEnable, int cutoffRaw);
    bool isEnabled() const;
    int  getCutoffPressureRaw() const;
    bool isRelayOn() const;

private:
    const uint8_t relayPin_;
    PressureSensorManager& pressureSensor_;

    bool enable_ = false;
    bool relayOn_     = false;
    int  cutoffRaw_   = 3800;
    int  hysteresisRaw_ = 100;

    void writeRelay_(bool on);
    void evaluateAutoControl_();
};