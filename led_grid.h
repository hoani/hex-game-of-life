#ifndef _HGOL_LED_GRID_H_
#define _HGOL_LED_GRID_H_

#include "grid.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

class LedGrid : public GridView
{
public:
    LedGrid(int pin);

    void update(const Grid *grid);

private:
    Adafruit_NeoPixel pixels;
};

#endif