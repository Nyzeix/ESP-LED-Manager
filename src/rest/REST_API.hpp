#pragma once
#include <cstdint>
#include <ESPAsyncWebServer.h>

#include "src/services/LedService.hpp"

#define DEBUG
class REST_API {
public:
    // Dependencies injection
    explicit REST_API(AsyncWebServer& server, LedService& ledService);

    void begin();
    // GET: status/
    void getStatus(AsyncWebServerRequest *);
    // POST: status/ // Status des lumières. ON/OFF
    void setStatus(AsyncWebServerRequest *);
    // GET: Colors/ // Tout si 0 // Reçoit et retourne JSON
    void getColors(AsyncWebServerRequest *);
    // POST: Colors/ // Tout si 0 // Reçoit et retourne JSON
    void setColors(AsyncWebServerRequest *);
    // GET: Brightness
    void getBrightness(AsyncWebServerRequest *);
    // POST: Brightness
    void setBrightness(AsyncWebServerRequest *);
    // GET: Time/
    void getTime(AsyncWebServerRequest *);
    // POST: UpdateTime/
    void updateTime(AsyncWebServerRequest *);
    // GET: TimeOut/
    void getTimeOut(AsyncWebServerRequest *);
    // POST: TimeOut/ // Format JSON: Status:"enable"/"disable", Duration:Integer
    void setTimeOut(AsyncWebServerRequest *);
    void handleParamsRequest(AsyncWebServerRequest *, uint8_t *, size_t, size_t, size_t);
    void homeRequest(AsyncWebServerRequest *request);
    void notFound(AsyncWebServerRequest *request);
private:
    AsyncWebServer& server;
    LedService& ledService;

#ifdef DEBUG
    void printJson(JsonVariant v, int indent = 0) {
        String padding = "";
        for (int i = 0; i < indent; i++) padding += "  ";

        if (v.is<JsonObject>()) {
            JsonObject obj = v.as<JsonObject>();
            for (JsonPair kv : obj) {
                Serial.print(padding);
                Serial.print(kv.key().c_str());
                Serial.println(":");
                printJson(kv.value(), indent + 1);
            }
        }
        else if (v.is<JsonArray>()) {
            JsonArray arr = v.as<JsonArray>();
            int index = 0;
            for (JsonVariant value : arr) {
                Serial.print(padding);
                Serial.println("[" + String(index++) + "]");
                printJson(value, indent + 1);
            }
        }
        else {
            Serial.print(padding);
            Serial.println(v.as<String>());
        }
    }
#endif
};


