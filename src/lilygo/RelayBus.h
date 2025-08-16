#pragma once
#include <Arduino.h>
#include <ShiftRegister74HC595.h>

class RelayBus {
public:
    RelayBus();

    void begin();
    void setChannel(uint8_t number, bool on);
    void clearAll();

private:
    static constexpr uint8_t PIN_DATA  = 7;  // DS
    static constexpr uint8_t PIN_CLOCK = 5;  // SHCP
    static constexpr uint8_t PIN_LATCH = 6;  // STCP
    static constexpr uint8_t PIN_OE    = 4;  // /OE

    ShiftRegister74HC595<1> sr;
};