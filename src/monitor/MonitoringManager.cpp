#include "MonitoringManager.h"

static inline float bytesToKB(size_t b)  { return b / 1024.0f; }
static inline float bytesToMB(size_t b)  { return b / (1024.0f * 1024.0f); }

void MonitoringManager::setup() {
    startFreeHeap_  = ESP.getFreeHeap();
    startFreePsram_ = ESP.getPsramSize() ? ESP.getFreePsram() : 0;
    lastLogMs_ = millis();
}

void MonitoringManager::loop(uint32_t cycleStartMillis) {
    if (cycleStartMillis - lastLogMs_ >= intervalMs_) {
        logOnce_();
        lastLogMs_ = cycleStartMillis;
    }
}

void MonitoringManager::logOnce_() {
    const size_t heapTotal   = ESP.getHeapSize();
    const size_t heapFree    = ESP.getFreeHeap();
    const size_t heapMinFree = ESP.getMinFreeHeap();
    const size_t heapMaxBlk  = ESP.getMaxAllocHeap();
    const size_t heapUsed    = heapTotal - heapFree;
    const float  heapFragPct = (heapFree > 0)
        ? (100.0f - (100.0f * (float)heapMaxBlk / (float)heapFree))
        : 0.0f;

    Serial.printf("HEAP  used/free/total: %.1f KB / %.1f KB / %.1f KB | minFree: %.1f KB | maxBlock: %.1f KB | frag≈%.0f%%\n",
                  bytesToKB(heapUsed), bytesToKB(heapFree), bytesToKB(heapTotal),
                  bytesToKB(heapMinFree), bytesToKB(heapMaxBlk), heapFragPct);

    if (startFreeHeap_ > 0) {
        const long delta = (long)heapFree - (long)startFreeHeap_;
        Serial.printf("HEAP  Δsince boot: %.1f KB\n", bytesToKB((size_t)abs(delta)) * (delta < 0 ? -1.0f : 1.0f));
    }

    // PSRAM (если доступна)
    const size_t psTotal = ESP.getPsramSize();
    if (psTotal > 0) {
        const size_t psFree    = ESP.getFreePsram();
        const size_t psMinFree = ESP.getMinFreePsram();
        const size_t psMaxBlk  = ESP.getMaxAllocPsram();
        const size_t psUsed    = psTotal - psFree;
        const float  psFragPct = (psFree > 0)
            ? (100.0f - (100.0f * (float)psMaxBlk / (float)psFree))
            : 0.0f;

        Serial.printf("PSRAM used/free/total: %.2f MB / %.2f MB / %.2f MB | minFree: %.2f MB | maxBlock: %.2f MB | frag≈%.0f%%\n",
                      bytesToMB(psUsed), bytesToMB(psFree), bytesToMB(psTotal),
                      bytesToMB(psMinFree), bytesToMB(psMaxBlk), psFragPct);

        if (startFreePsram_ > 0) {
            const long deltaPs = (long)psFree - (long)startFreePsram_;
            Serial.printf("PSRAM Δsince boot: %.2f MB\n", bytesToMB((size_t)abs(deltaPs)) * (deltaPs < 0 ? -1.0f : 1.0f));
        }
    } else {
        Serial.printf("PSRAM not present\n");
    }
}