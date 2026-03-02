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

    server.on("/Colors/Row/{id}", HTTP_GET, [this](AsyncWebServerRequest* request) {
        this->getRowColor(request);
    });

    server.on("/Colors/Pixel/{id}", HTTP_GET, [this](AsyncWebServerRequest* request) {
        this->getPixelColor(request);
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
    uint8_t rows = this->ledService.getRows();
    uint8_t brightness = this->ledService.getBrightness();

    StaticJsonDocument<1024> doc;
    JsonObject color = doc.createNestedObject("Color");

    for (uint8_t i = 1; i <= rows; i++) {
        uint8_t r = 0, g = 0, b = 0;
        this->ledService.getRowColor(i, r, g, b);

        String key = "Row" + String(i);
        JsonObject rowObj = color.createNestedObject(key);
        rowObj["red"] = r;
        rowObj["green"] = g;
        rowObj["blue"] = b;
        rowObj["brightness"] = brightness;
    }

    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
}

// POST: Colors/ // Tout si 0 // Reçoit et retourne JSON
void REST_API::setColors(AsyncWebServerRequest *request) {
}


// GET: Colors/Row/{id} // Retourne JSON avec la couleur du ruban sélectionné
void REST_API::getRowColor(AsyncWebServerRequest *request) {
    String idStr = request->pathArg(0);
    int id = idStr.toInt();

    if (id <= 0) {
        request->send(400, "application/json", "{\"error\":\"Invalid row id\"}");
        return;
    }

    uint8_t r = 0, g = 0, b = 0;
    if (!this->ledService.getRowColor((uint8_t)id, r, g, b)) {
        request->send(404, "application/json", "{\"error\":\"Row not found\"}");
        return;
    }

    uint8_t brightness = this->ledService.getBrightness();

    StaticJsonDocument<128> doc;
    JsonObject row = doc.createNestedObject("Row");
    row["red"] = r;
    row["green"] = g;
    row["blue"] = b;
    row["brightness"] = brightness;

    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
}


// GET: Colors/Pixel/{id} // Retourne JSON avec la couleur du pixel sélectionné
void REST_API::getPixelColor(AsyncWebServerRequest *request) {
    String idStr = request->pathArg(0);
    int id = idStr.toInt();

    if (id < 0 || (uint16_t)id >= this->ledService.getPixels()) {
        request->send(400, "application/json", "{\"error\":\"Invalid pixel id\"}");
        return;
    }

    uint8_t r = 0, g = 0, b = 0;
    this->ledService.getColor((uint16_t)id, r, g, b);
    uint8_t brightness = this->ledService.getBrightness();

    StaticJsonDocument<128> doc;
    JsonObject pixel = doc.createNestedObject("pixel");
    pixel["red"] = r;
    pixel["green"] = g;
    pixel["blue"] = b;
    pixel["brightness"] = brightness;

    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
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