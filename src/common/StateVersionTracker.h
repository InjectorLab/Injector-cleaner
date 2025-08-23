#pragma once
#include <Arduino.h>

class StateVersionTracker {
public:
    StateVersionTracker() = default;

    uint32_t getVersion() const {
        return version_;
    }

protected:
    void bumpVersion() {
        ++version_;
        if (version_ >= ROLL_LIMIT) {
            version_ = 0;
        }
    }

private:
    static constexpr uint32_t ROLL_LIMIT = 1000000;
    uint32_t version_ = 1;
};