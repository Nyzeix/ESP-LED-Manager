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


void LedService::getColor(uint8_t &r, uint8_t &g, uint8_t &b) {
    strip.getColor(0, r, g, b);
}


void LedService::getColor(uint16_t pixel, uint8_t &r, uint8_t &g, uint8_t &b) {
    strip.getColor(pixel, r, g, b);
}


bool LedService::getRowColor(uint8_t row, uint8_t &r, uint8_t &g, uint8_t &b) {
    if (row == 0 || row > strip.getRows()) {
        return false;
    }
    strip.getRowColor(row, r, g, b);
    return true;
}


void LedService::setBrightness(uint8_t brightness) {
    strip.setBrightness(brightness);
    strip.show();
}


uint8_t LedService::getBrightness() {
    return strip.getBrightness();
}


uint8_t LedService::getRows() {
    return strip.getRows();
}


uint16_t LedService::getPixels() {
    return strip.getPixels();
}


void LedService::show() {
    strip.show();
}


void LedService::clear() {
    strip.clear();
}