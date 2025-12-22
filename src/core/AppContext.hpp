#pragma once

#include <ESPAsyncWebServer.h>
#include "src/services/LedService.hpp"
#include "src/rest/REST_API.hpp"

class AppContext {
public:
    static void init();
private:
    // Dependencies injection
    static AsyncWebServer server;
    static LedService ledService;
    static REST_API restApi;
};
