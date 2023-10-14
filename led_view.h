#ifndef _HGOL_LED_VIEW_H_
#define _HGOL_LED_VIEW_H_

#include "grid.h"

#include <WS2812Serial.h>

#define NUM_CELL_HUES (6)

const uint16_t cellHueOptions[NUM_CELL_HUES] = {
    0x8800, // Bluish.
    0x1a00, // Orangish.
    0xaa00, // Deeper Blue.
    0xbe00, // Purpleish.
    0x2800, // Yellowish.
    0x8000, // Aqua.
};

class LedView : public GridView
{
public:
    LedView(int pin);

    void update(const Grid *grid, float progress);

private:
    WS2812Serial leds;
    const static uint16_t killHue = 0x0000;  // Red.
    const static uint16_t spawnHue = 0x5555; // Green.
    uint16_t cellHue = 0x8800;               // Bluish.
    uint8_t sat = 0xff;
    uint8_t cellSat = 0xff;
    uint8_t val = 0xb0;

    uint32_t calculateKillColor(float progress);
    uint32_t calculateSpawnColor(float progress);
};

#endif