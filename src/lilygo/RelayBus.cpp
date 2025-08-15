#include "RelayBus.h"

void RelayBus::begin() {
    pinMode(PIN_DATA, OUTPUT);
    pinMode(PIN_CLOCK, OUTPUT);
    pinMode(PIN_LATCH, OUTPUT);
    pinMode(PIN_OE, OUTPUT);
    digitalWrite(PIN_OE, LOW);
    write_();
}

void RelayBus::setChannel(uint8_t ch, bool on) {
    const uint8_t bit = bitFor_(ch);
    if (!bit) return;
    if (on) state_ |= bit; else state_ &= ~bit;
    write_();
}

void RelayBus::applyMasked(uint8_t mask, uint8_t value) {
    state_ = (uint8_t)((state_ & ~mask) | (value & mask));
    write_();
}

void RelayBus::clearAll() {
    state_ = 0;
    write_();
}

void RelayBus::write_() {
    digitalWrite(PIN_LATCH, LOW);
    shiftOut(PIN_DATA, PIN_CLOCK, MSBFIRST, state_);
    digitalWrite(PIN_LATCH, HIGH);
}