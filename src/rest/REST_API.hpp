#pragma once
#include <cstdint>
#include <ESPAsyncWebServer.h>

#include "src/services/LedService.hpp"

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
    // GET: Time/
    void getTime(AsyncWebServerRequest *);
    // POST: UpdateTime/
    void updateTime(AsyncWebServerRequest *);
    // GET: TimeOut/
    void getTimeOut(AsyncWebServerRequest *);
    // POST: TimeOut/ // Format JSON: Status:"enable"/"disable", Duration:Integer
    void setTimeOut(AsyncWebServerRequest *);
    void myFunction(AsyncWebServerRequest *, uint8_t *, size_t, size_t, size_t);
    void homeRequest(AsyncWebServerRequest *request);
    void notFound(AsyncWebServerRequest *request);
private:
    AsyncWebServer& server;
    LedService& ledService;
};


