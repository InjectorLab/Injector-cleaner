#include "RelayBus.h"

RelayBus::RelayBus()
    : sr(PIN_DATA, PIN_CLOCK, PIN_LATCH) {}

void RelayBus::begin() {
    pinMode(PIN_OE, OUTPUT);
    digitalWrite(PIN_OE, LOW);
    clearAll();
}

void RelayBus::setChannel(uint8_t number, bool on) {
    sr.set(number, on ? HIGH : LOW);
}

void RelayBus::clearAll() {
    sr.setAllLow();
}