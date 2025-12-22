//
// Created by a058446 on 19/12/2025.
//

#ifndef MAIN_LEDSERVICE_HPP
#define MAIN_LEDSERVICE_HPP

#pragma once
#include "src/hardware/LedStrip.hpp"

class LedService {
public:
    void init();
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness);
    void setBrightness(uint8_t brightness);
    uint8_t getBrightness();

private:
    // Add Strip here
    LedStrip strip{4, 10, 5}; // pin, nb leds, nb cln
};


#endif //MAIN_LEDSERVICE_HPP