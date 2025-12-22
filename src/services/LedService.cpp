#include "LedService.hpp"

void LedService::init() {
    strip.begin();
}


void LedService::setColor(uint8_t r, uint8_t g, uint8_t b) {
    strip.setColor(r, g, b);
    strip.show();
}


bool LedService::setColor(uint8_t row, uint8_t r, uint8_t g, uint8_t b) {
    if(row > strip.getRows()) {
        // Row selected not existing.
        return false;
    }
    strip.setRowColor(row, r, g, b);
}


void LedService::getColor(uint16_t pin, uint8_t &r, uint8_t &g, uint8_t &b) {
    uint32_t color = strip.getPixelColor(pin);
    // What is the format of "color" ?
    // Probably: RGBb. Should be splitted.
}


void LedService::setBrightness(uint8_t brightness) {
    strip.setBrightness(brightness);
    strip.show();
}


uint8_t LedService::getBrightness() {
    return strip.getBrightness();
}


void LedService::show() {
    strip.show();
}


void LedService::clear() {
    strip.clear();
}