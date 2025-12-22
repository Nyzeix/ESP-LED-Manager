#include "LedService.hpp"

void LedService::init() {
    strip.begin();
}

void LedService::setColor(uint8_t r, uint8_t g, uint8_t b) {
    strip.setColor(r, g, b);
    strip.show();
}

void LedService::getColor(uint8_t &r, uint8_t &g, uint8_t &b, uint16_t pin) {
    uint32_t color = strip.getColor(pin);
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