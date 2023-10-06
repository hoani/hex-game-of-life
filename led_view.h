#ifndef _HGOL_LED_VIEW_H_
#define _HGOL_LED_VIEW_H_

#include "grid.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

class LedView : public GridView
{
public:
    LedView(int pin);

    void update(const Grid *grid);

private:
    Adafruit_NeoPixel pixels;
    const static int NUM_HUES = 8;
    const static uint16_t cellHue = 0x8800;  // Bluish.
    const static uint16_t killHue = 0x0000;  // Red.
    const static uint16_t spawnHue = 0x5555; // Green.
    uint8_t sat = 0xff;
    uint8_t val = 0xb0;
};

#endif