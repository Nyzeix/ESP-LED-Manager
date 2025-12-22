#pragma once
#include <cstdint>
#include <Adafruit_NeoPixel.h>

#define ON 1
#define OFF 0

class LedStrip {
private:
    Adafruit_NeoPixel strip;
    uint16_t pixels;
    uint8_t totalRows;
    uint8_t pixelsPerRow;

    void setPixels(uint16_t p) {pixels = p;}
    void setRows(uint8_t i) {totalRows = i;}
    void setPixelsPerRow(uint8_t p) {pixelsPerRow = p;}
public:
    LedStrip(int pin, int pixels);
    LedStrip(int pin, int pixelsPerRow, int rows);

    uint8_t getPixelsPerRow() const {return pixelsPerRow;}
    uint8_t getRows() const {return totalRows;}
    uint16_t getPixels() const {return pixels;}

    bool begin();
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void getColor(uint16_t pixel, uint8_t r, uint8_t g, uint8_t b);
    //void setColor(uint16_t index, uint8_t r, uint8_t g, uint8_t b);
    void setRowColor(uint8_t row, uint8_t r, uint8_t g, uint8_t b);
    void getRowColor(uint8_t row, uint8_t r, uint8_t g, uint8_t b);
    uint32_t getPixelColor(uint16_t pixel);
    void setBrightness(uint8_t brightness);
    uint8_t getBrightness();
    void clear();
    void show();


    void hexToRgb(String, int*, int*, int*, int*);
    String rgbToHex(int, int, int, int); // TODO: must be replaced by char[].

};
