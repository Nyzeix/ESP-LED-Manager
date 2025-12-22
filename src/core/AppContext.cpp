#include <Arduino.h>
#include "src/core/AppContext.hpp"
#include "src/services/Network.hpp"
//#include "src/storage/ConfigStorage.hpp"

AsyncWebServer AppContext::server(80);
LedService AppContext::ledService;
REST_API AppContext::restApi{server, ledService };

void AppContext::init() {
    Serial.begin(115200);
    WifiService::connect();
    //ConfigStorage::load(); // Preferences
    ledService.init();
    restApi.begin();
    server.begin(); // Se référer à la doc de AsyncWebServer
}
