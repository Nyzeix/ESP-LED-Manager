#include <Arduino.h>

#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESPmDNS.h>

#include <src/services/Network.hpp>

// Connectivity stuffs
const char* ssid = "ssid";
const char* pswd = "pswd";

//Wi-Fi
IPAddress ip(192, 168, 1, 253);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

void WifiService::connect() {
    WiFi.config(ip, gateway, subnet);
    Serial.print("Connexion à ");
    Serial.println(ssid);
    WiFi.begin(ssid, pswd);
    int i = 1;
    while (WiFi.status() != WL_CONNECTED) {
        delay(200);
        Serial.print('.');
    }
    Serial.println("Connected");
    Serial.println(WiFi.localIP());
}