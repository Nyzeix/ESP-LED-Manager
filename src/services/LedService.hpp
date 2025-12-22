#pragma once
#include "src/hardware/LedStrip.hpp"

class LedService {
public:
    void init();
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    bool setColor(uint8_t row, uint8_t r, uint8_t g, uint8_t b);
    void getColor(uint8_t &r, uint8_t &g, uint8_t &b);
    void getColor(uint16_t pin, uint8_t &r, uint8_t &g, uint8_t &b);
    void setBrightness(uint8_t brightness);
    uint8_t getBrightness();

    void show();
    void clear();

private:
    // Add Strip here FOR NOW.
    //TODO: Get LedStrip data/value from Preferences (Add new/delete with rest request).
    LedStrip strip{4, 10, 5}; // pin, nb leds, nb cln
};