#include "OtaManager.h"

void OtaManager::setup() {
    if (hostname_ && strlen(hostname_) > 0) {
        ArduinoOTA.setHostname(hostname_);
    }

    ArduinoOTA
        .onStart([this]() {
            Serial.println(F("[OTA] Start"));
        })
        .onEnd([this]() {
            Serial.println(F("[OTA] End"));
        })
        .onError([this](ota_error_t error) {
            Serial.printf("[OTA] Error[%u]: ", static_cast<unsigned>(error));
            switch (error) {
                case OTA_AUTH_ERROR:    Serial.println(F("Auth Failed")); break;
                case OTA_BEGIN_ERROR:   Serial.println(F("Begin Failed")); break;
                case OTA_CONNECT_ERROR: Serial.println(F("Connect Failed")); break;
                case OTA_RECEIVE_ERROR: Serial.println(F("Receive Failed")); break;
                case OTA_END_ERROR:     Serial.println(F("End Failed")); break;
                default:                Serial.println(F("Unknown Error")); break;
            }
        });

    ArduinoOTA.begin();
    Serial.printf("[OTA] Ready. Hostname: %s\n", hostname_ ? hostname_ : "(none)");
}

void OtaManager::loop() {
    ArduinoOTA.handle();
}