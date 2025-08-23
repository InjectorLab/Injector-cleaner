#include <Arduino.h>

#include "lilygo/RelayBus.h"

#include "net/WiFiConnector.h"

#include "sensors/PressureSensorManager.h"
#include "sensors/adapters/PinAdcPressureSensorAdapter.h"

#include "pump/PumpManager.h"
#include "pump/adapters/LedcPumpAdapter.h"

#include "injector/adapters/RelayBusInjectorAdapter.h"
#include "injector/InjectorManager.h"

#include "net/WebSocketManager.h"
#include "common/LifeCycleHandler.h"
#include "timer/TimerManager.h"  

#include "ota/OtaManager.h"

#include "monitor/MonitoringManager.h"

#include "secret.h"

// ================== USER CONFIG ==================
static const uint16_t DEFAULT_DELAY_MS = 100;
static const uint16_t DEFAULT_PULSE_MS = 100;
// =================================================
WiFiConnector          wifi(WIFI_SSID, WIFI_PASS);
PinAdcPressureSensorAdapter pressureAdapter(10);
PressureSensorManager  pressure(pressureAdapter);

LedcPumpAdapter pumpAdapter(14, false);

PumpManager            pump(pumpAdapter, pressure);

RelayBus relayBus;
RelayBusInjectorAdapter inj1(relayBus, 2, true);
RelayBusInjectorAdapter inj2(relayBus, 3, true);
RelayBusInjectorAdapter inj3(relayBus, 4, true);
RelayBusInjectorAdapter inj4(relayBus, 5, true);

InjectorManager        injector(DEFAULT_DELAY_MS, DEFAULT_PULSE_MS);

TimerManager           timer(injector);
WebSocketManager       websocket(pressure, pump, injector, timer);

OtaManager              ota("injector");

// MonitoringManager       monitoringManager(5000);

LifeCycleHandler* components[] = {
    &wifi, &ota, &pressure, &pump, &injector, &timer, &websocket
};

void setup() {
    Serial.begin(115200);

    delay(50);

    injector.addInjector(inj1);
    injector.addInjector(inj2);
    injector.addInjector(inj3);
    injector.addInjector(inj4);

    for (auto* component : components) component->setup();
}

void loop() {
    const uint32_t now = millis();
    for (auto* component : components) component->loop(now);
}