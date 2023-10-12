#ifndef _HGOL_LED_VIEW_H_
#define _HGOL_LED_VIEW_H_

#include "grid.h"

#include <WS2812Serial.h>

class LedView : public GridView
{
public:
    LedView(int pin);

    void update(const Grid *grid, float progress);

private:
    WS2812Serial leds;
    const static int NUM_HUES = 8;
    const static uint16_t cellHue = 0x8800;  // Bluish.
    const static uint16_t killHue = 0xffff;  // Red.
    const static uint16_t spawnHue = 0x5555; // Green.
    uint8_t sat = 0xff;
    uint8_t cellSat = 0xff;
    uint8_t val = 0xb0;

    uint32_t calculateKillColor(float progress);
    uint32_t calculateSpawnColor(float progress);
};

#endif