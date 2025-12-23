#include <ArduinoJson.h>

#include "src/rest/REST_API.hpp"

REST_API::REST_API(AsyncWebServer& server, LedService &ledService)
    : server(server), ledService(ledService) {}

void REST_API::begin() {
    // Init the server
    // Init routes
    /* Définition des fonctions voulues
    Obtenir le status: Eteint & allumé (GET)
    Obtenir le status: Les valeurs de couleurs de chaque rubans (RGB + brightness) (GET)
    Définir allumé / éteint seulement (POST)
    Définir valeurs de couleurs et luminosité de chaque rubans (POST) (Un JSON séparé avec {ruban: "x" {value}} {[...]} )

    Définir un horaire d'allumage et d'extinction (POST)

    Obtenir temps actuel sur ESP32 (GET)
    Demander une mise-à-jour du temps réel (POST)
    */

    // TEST
    server.on("/led/on", HTTP_GET, [this](AsyncWebServerRequest* request) {
        this->ledService.setColor(255, 255, 255);
        request->send(200, "text/plain", "LED ON");
    });

    server.on("/led/off", HTTP_GET, [this](AsyncWebServerRequest* request) {
        this->ledService.setColor(0, 0, 0);
        request->send(200, "text/plain", "LED OFF");
    });


    server.on("/", HTTP_GET, [this](AsyncWebServerRequest* request) {
        this->homeRequest(request);
    });

    //server.on("/status")
    /*
    server.on("/item", HTTP_GET, getRequest);
    server.on("/item", HTTP_POST, [](AsyncWebServerRequest * request){}, NULL, postRequest);
    server.on("/item", HTTP_PUT, [](AsyncWebServerRequest * request){}, NULL, putRequest);
    server.on("/item", HTTP_PATCH, [](AsyncWebServerRequest * request){}, NULL, patchRequest);
    server.on("/item", HTTP_DELETE, deleteRequest);*/

    server.on("/status", HTTP_GET, [this](AsyncWebServerRequest* request) {
        this->getStatus(request);
    });

    server.on("/status", HTTP_POST, [this](AsyncWebServerRequest* request) {
        this->setStatus(request);
    });

    server.on("/Colors", HTTP_GET, [this](AsyncWebServerRequest* request) {
        this->getColors(request);
    });

    server.on("/Colors", HTTP_POST, [this](AsyncWebServerRequest* request) {
        this->setColors(request);
    });

    server.on("/Brightness", HTTP_GET, [this](AsyncWebServerRequest* request) {
        this->getBrightness(request);
    });

    server.on("/Brightness", HTTP_POST, [this](AsyncWebServerRequest* request) {
        this->setBrightness(request);
    });

    server.on("/Time", HTTP_GET, [this](AsyncWebServerRequest* request) {
        this->getTime(request);
    });

    server.on("/UpdateTime", HTTP_POST, [this](AsyncWebServerRequest* request) {
        this->updateTime(request);
    });

    server.on("/TimeOut", HTTP_GET, [this](AsyncWebServerRequest* request) {
        this->getTimeOut(request);
    });

    server.on("/TimeOut", HTTP_POST, [this](AsyncWebServerRequest* request) {
        this->setTimeOut(request);
    });

    server.on("/setParams", HTTP_POST,
    [](AsyncWebServerRequest *request) {
        // rien à faire ici (handler principal)
    },
    nullptr,
    [this](AsyncWebServerRequest *request,
           uint8_t *data,
           size_t len,
           size_t index,
           size_t total) {
        this->handleParamsRequest(request, data, len, index, total);
    });

    server.onNotFound([this](AsyncWebServerRequest* request) {
        this->notFound(request);
    });

    server.begin();
    Serial.println("HTTP server started");
}

void REST_API::homeRequest(AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hello, world");
}

void REST_API::notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

// GET: status/
void REST_API::getStatus(AsyncWebServerRequest *request) {
    uint8_t brightness = this->ledService.getBrightness();
    String status = brightness ? "true" : "false";
    String output = "Status: " + String(status) + "\n";
    request->send(200, "text.plain", output);
}

// POST: status/ // Status des lumières. ON/OFF
void REST_API::setStatus(AsyncWebServerRequest *request) {
}

// GET: Colors/ // Tout si 0 // Retourne JSON
void REST_API::getColors(AsyncWebServerRequest *request) {

}

// POST: Colors/ // Tout si 0 // Reçoit et retourne JSON
void REST_API::setColors(AsyncWebServerRequest *request) {
}


void REST_API::getBrightness(AsyncWebServerRequest *request) {
}


void REST_API::setBrightness(AsyncWebServerRequest *request) {
}


// GET: Time/
void REST_API::getTime(AsyncWebServerRequest *request) {}

// POST: UpdateTime/
void REST_API::updateTime(AsyncWebServerRequest *request) {}

// GET: TimeOut/
void REST_API::getTimeOut(AsyncWebServerRequest *request) {}

// POST: TimeOut/ // Format JSON: Status:"enable"/"disable", Duration:Integer
void REST_API::setTimeOut(AsyncWebServerRequest *request) {}


void REST_API::handleParamsRequest(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, data);

    if (error) {
        request->send(400, "application/json", "{\"error\": " + String(error.c_str()) + "}");
        return;
    }
#ifdef DEBUG
    printJson(doc);
#endif
    if (doc.containsKey("rows") && doc["rows"].is<JsonArray>()) {

        JsonArray rows = doc["rows"];

        for (JsonObject row : rows) {

            uint8_t id = row["id"] | -1;
            uint8_t brightness = row["br"] | row["brightness"] | 255;

            JsonArray rgb = row["rgb"];
            uint8_t r = rgb[0] | 0;
            uint8_t g = rgb[1] | 0;
            uint8_t b = rgb[2] | 0;

            if (id > 0) {
                this->ledService.setColor(id, r, g, b);
                this->ledService.setBrightness(brightness);
            }
        }
    }
    else if (doc.containsKey("global") && doc["global"].is<JsonObject>()) {

        JsonObject global = doc["global"];

        uint8_t brightness = global["br"] | global["brightness"] | 255;

        JsonObject color = global["color"];
        uint8_t r = color["red"] | 0;
        uint8_t g = color["green"] | 0;
        uint8_t b = color["blue"] | 0;

        this->ledService.setBrightness(brightness);
        this->ledService.setColor(r, g, b);
    }
    else {
        request->send(400, "text/plain", "error in json data. No valid data found.");
        return;
    }

    request->send(200, "application/json", "{\"status\":\"ok\"}");
};