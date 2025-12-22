#pragma once
#include <cstdint>
#include <Adafruit_NeoPixel.h>

#define ON 1
#define OFF 0

class LedStrip {
public:
    LedStrip(int pin, int pixels);
    LedStrip(int pin, int pixelsPerRow, int rows);

    bool begin();
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void getColor(uint16_t pixel, uint8_t r, uint8_t g, uint8_t b);
    //void setColor(uint16_t index, uint8_t r, uint8_t g, uint8_t b);
    void setRowColor(uint8_t row, uint8_t r, uint8_t g, uint8_t b);
    void getRowColor(uint8_t row, uint8_t r, uint8_t g, uint8_t b);
    void setBrightness(uint8_t brightness);
    uint8_t getBrightness();
    void clear();
    void show();

    void hexToRgb(String, int*, int*, int*, int*);
    String rgbToHex(int, int, int, int); // TODO: must be replaced by char[].

private:
    Adafruit_NeoPixel strip;
    uint16_t pixels;
    uint8_t totalRows;
    uint8_t pixelsPerRow;

    void setPixels(int p) {pixels = p;}
    int getPixels() const {return pixels;}
    void setRows(int i) {totalRows = i;}
    int getRows() const {return totalRows;}
    void setPixelsPerRow(int p) {pixelsPerRow = p;}
    int getPixelsPerRow() const {return pixelsPerRow;}
};
