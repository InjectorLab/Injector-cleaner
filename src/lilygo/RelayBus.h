#pragma once
#include <Arduino.h>

class RelayBus {
public:
    void begin();

    void setChannel(uint8_t channel1to8, bool on);

    void applyMasked(uint8_t mask, uint8_t value);

    void clearAll();

    uint8_t getState() const { return state_; }

private:
    static constexpr uint8_t PIN_DATA  = 7;  // DS
    static constexpr uint8_t PIN_CLOCK = 5;  // SHCP
    static constexpr uint8_t PIN_LATCH = 6;  // STCP
    static constexpr uint8_t PIN_OE    = 4;  // /OE

    uint8_t state_ = 0;

    void write_();

    static inline uint8_t bitFor_(uint8_t ch) { return (ch >= 1 && ch <= 8) ? (1u << (ch - 1)) : 0; }
};