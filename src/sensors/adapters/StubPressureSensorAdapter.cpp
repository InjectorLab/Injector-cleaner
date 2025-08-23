#include "StubPressureSensorAdapter.h"

StubPressureSensorAdapter::StubPressureSensorAdapter(uint16_t value)
: value_(value) {}

void StubPressureSensorAdapter::setup() {}

uint16_t StubPressureSensorAdapter::readRaw() {
    return value_;
}