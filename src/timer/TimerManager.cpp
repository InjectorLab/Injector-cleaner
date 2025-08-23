#include "TimerManager.h"

TimerManager::TimerManager(InjectorManager& injector)
: injector_(injector) {}

void TimerManager::setup() {}

void TimerManager::loop(uint32_t cycleStartMillis) {
    if (!running_) {
        return;
    }

    const uint32_t endAt = startedAtMs_ + durationMs_;

    if ((int32_t)(cycleStartMillis - endAt) >= 0) {
        stop();
    }
}

void TimerManager::start(uint32_t durationMs) {
    startedAtMs_ = millis();
    durationMs_  = durationMs;
    running_     = true;

    startInjectorsInfinite_();

    bumpVersion();
}

void TimerManager::stop() {
    if (!running_) {
        stopInjectors_();
        return;
    }
    running_ = false;
    stopInjectors_();

    bumpVersion();
}

bool TimerManager::isRunning() const {
    return running_;
}

uint32_t TimerManager::getRemainingMs() const {
    if (!running_) return 0;
    const uint32_t now = millis();
    const uint32_t endAt = startedAtMs_ + durationMs_;
    if ((int32_t)(now - endAt) >= 0) return 0;
    return endAt - now;
}

void TimerManager::startInjectorsInfinite_() {
    const uint8_t count = injector_.getInjectorCount();
    if (count == 0) {
        return;
    }

    uint8_t pattern[InjectorManager::kMaxPattern];
    const uint8_t n = (count > InjectorManager::kMaxPattern)
                      ? InjectorManager::kMaxPattern
                      : count;
    for (uint8_t i = 0; i < n; ++i) pattern[i] = i + 1;

    injector_.startPattern(pattern, n, -1);
}

void TimerManager::stopInjectors_() {
    injector_.stopPattern();
}