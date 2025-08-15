#include <Arduino.h>

#include "net/WiFiConnector.h"

#include "sensors/PressureSensorManager.h"
#include "sensors/adapters/PinAdcPressureSensorAdapter.h"

#include "pump/PumpManager.h"
#include "pump/adapters/PinRelayPumpAdapter.h"

#include "injector/adapters/PinInjectorAdapter.h"
#include "injector/InjectorManager.h"

#include "net/WebSocketManager.h"
#include "common/LifeCycleHandler.h"
#include "timer/TimerManager.h"  

// ================== USER CONFIG ==================

static const uint16_t DEFAULT_DELAY_MS = 10;
static const uint16_t DEFAULT_PULSE_MS = 10;
// =================================================

// WiFiConnector          wifi(WIFI_SSID, WIFI_PASS);
PinAdcPressureSensorAdapter pressureAdapter(10);
PressureSensorManager  pressure(pressureAdapter);

PinRelayPumpAdapter         pumpAdapter(9, true);
PumpManager            pump(pumpAdapter, pressure);

PinInjectorAdapter          inj1Adapter(11, true);
PinInjectorAdapter          inj2Adapter(12, true);
PinInjectorAdapter          inj3Adapter(13, true);
PinInjectorAdapter          inj4Adapter(14, true);
InjectorManager        injector(DEFAULT_DELAY_MS, DEFAULT_PULSE_MS);

TimerManager           timer(injector);
// WebSocketManager       websocket(pressure, pump, injector, timer);

// LifeCycleHandler* components[] = {
//     &wifi, &pressure, &pump, &injector, &timer, &websocket
// };

LifeCycleHandler* components[] = {
    &pressure, &pump, &injector, &timer
};

void setup() {
    Serial.begin(115200);
    delay(50);

    injector.addInjector(inj1Adapter);
    injector.addInjector(inj2Adapter);
    injector.addInjector(inj3Adapter);
    injector.addInjector(inj4Adapter);

    timer.start(10000);

    for (auto* component : components) component->setup();
}

void loop() {
    for (auto* component : components) component->loop();
}