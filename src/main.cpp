#include <Arduino.h>

#include "net/WiFiConnector.h"
#include "sensors/PressureSensorManager.h"
#include "pump/PumpManager.h"
#include "injector/InjectorManager.h"
#include "net/WebSocketManager.h"
#include "common/LifeCycleHandler.h"
#include "timer/TimerManager.h"  

// ================== USER CONFIG ==================
static const char* WIFI_SSID = "Change-Me";
static const char* WIFI_PASS = "Change-Me";

static const uint8_t PIN_PUMP_RELAY   = 14;
static const uint8_t PIN_PRESSURE_ADC = 12;

static const uint8_t PIN_INJ_1 = 11;
static const uint8_t PIN_INJ_2 = 10;
static const uint8_t PIN_INJ_3 = 13;
static const uint8_t PIN_INJ_4 = 46;

static const uint16_t DEFAULT_DELAY_MS = 10;
static const uint16_t DEFAULT_PULSE_MS = 10;
// =================================================

WiFiConnector          wifi(WIFI_SSID, WIFI_PASS);
PressureSensorManager  pressure(PIN_PRESSURE_ADC);
PumpManager            pump(PIN_PUMP_RELAY, pressure);
InjectorManager        injector(DEFAULT_DELAY_MS, DEFAULT_PULSE_MS);
TimerManager           timer(injector);
WebSocketManager       websocket(pressure, pump, injector, timer);

LifeCycleHandler* components[] = {
    &wifi, &pressure, &pump, &injector, &timer, &websocket
};

void setup() {
    Serial.begin(115200);
    delay(50);

    injector.addInjectorPin(PIN_INJ_1);
    injector.addInjectorPin(PIN_INJ_2);
    injector.addInjectorPin(PIN_INJ_3);
    injector.addInjectorPin(PIN_INJ_4);

    for (auto* component : components) component->setup();
}

void loop() {
    for (auto* component : components) component->loop();
}