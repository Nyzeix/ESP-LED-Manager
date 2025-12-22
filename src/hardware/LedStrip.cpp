#include <Arduino.h>

#include <Adafruit_NeoPixel.h>

#include <src/hardware/LedStrip.hpp>


LedStrip::LedStrip(int pin, int pixels)
    : strip(pixels, pin, NEO_GRB + NEO_KHZ800) {
    setPixels(pixels);
    setRows(1);
    setPixelsPerRow(pixels);
}


LedStrip::LedStrip(int pin, int pixelsPerRow, int rows)
    : strip(pixelsPerRow*rows, pin, NEO_GRB + NEO_KHZ800) {
    setPixels(pixelsPerRow*rows);
    setRows(rows);
    setPixelsPerRow(pixelsPerRow);
}


bool LedStrip::begin() {
    if (!strip.begin()) {
        return false;
    }
    strip.clear();
    strip.show();
    return true;
}


void LedStrip::setColor(uint8_t r, uint8_t g, uint8_t b) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(r, g, b));
    }
}


void LedStrip::getColor(uint16_t pixel, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t color = strip.getPixelColor(pixel);
    // TODO: Convert color back to 3 value as Red/Green/Blue

}


void LedStrip::setRowColor(uint8_t row, uint8_t r, uint8_t g, uint8_t b) {
    // For row number x, update led from row x-1 to row x.
    uint16_t endingPixel = row * this->getPixelsPerRow();
    for (uint16_t i = endingPixel - this->getPixelsPerRow(); i < endingPixel; i++) {
        strip.setPixelColor(i, strip.Color(r, g, b));
    }
}


void LedStrip::getRowColor(uint8_t row, uint8_t r, uint8_t g, uint8_t b) {
    //TODO
}


uint32_t LedStrip::getPixelColor(uint16_t pixel) {
    return strip.getPixelColor(pixel);
}


void LedStrip::setBrightness(uint8_t brightness) {
    strip.setBrightness(brightness);
}


uint8_t LedStrip::getBrightness() {
    return strip.getBrightness();
}


void LedStrip::clear() {
    strip.clear();
}


void LedStrip::show() {
    strip.show();
}


/**
 *
 * @param hexValue: Value to be converted. format: #RRGGBB or #RRGGBBbb (with brightness level)
 * @param red: variable to put in red level
 * @param green: variable to put in green level
 * @param blue: variable to put in blue level
 * @param brightness : variable to put in brightness level. Return the previous level if not mentionned in hexValue
 */
void LedStrip::hexToRgb(String hexValue, int* red, int* green, int* blue, int* brightness) {
    if (hexValue[0] == '#') {
        hexValue = hexValue.substring(1);
    }
    // #RRGGBB, take previous brightness level
    if (hexValue.length() != 6) {
        *brightness = getBrightness();
    }
    // #RRGGBBbb
    else if (hexValue.length() != 8) {
        return;
    }
    long intValue = strtoul(hexValue.c_str(), NULL, 16);

    // R, G, B and Brightness
    *red = (int)(intValue >> 24) & 0xFF;
    *green = (int)(intValue >> 16) & 0xFF;
    *blue = (int)(intValue >> 8) & 0xFF;
    *brightness = (int)intValue & 0xFF;
}

/**
 *
 * @param red: Red level to be converted
 * @param green: Green level to be converted
 * @param blue: Blue level to be converted
 * @param bn: Brightness level to be converted
 * @return: A hexadecimal value as a string.
 */
String LedStrip::rgbToHex(int red, int green, int blue, int  bn) {
    // Vérifier si les valeurs RGB sont valides (0-255)
    if (red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255) {
        return "000000";
    }

    long hexValue = ((long)red << 24) | ((long)green << 16) | ((long)blue << 8) | bn;

    char hexString[9];
    sprintf(hexString, "%06lX", hexValue);

    return String(hexString);
}