#include "led_view.h"
#include <Adafruit_NeoPixel.h>

const static int NUM_GRID_PIXELS = 113;
const static int GRID_ROW_OFFSET[ROWS] = {0, 9, 20, 33, 48, 65, 80, 93, 104};

static byte drawingMemory[NUM_GRID_PIXELS * 3];         //  3 bytes per LED
static DMAMEM byte displayMemory[NUM_GRID_PIXELS * 12]; // 12 bytes per LED

static uint32_t hsv(uint16_t h, uint8_t s, uint8_t v)
{
    return Adafruit_NeoPixel::gamma32(Adafruit_NeoPixel::ColorHSV(h, s, v));
}

LedView::LedView(int pin) : leds(NUM_GRID_PIXELS, displayMemory, drawingMemory, pin, WS2812_GRB)
{
    leds.begin();
}

void LedView::update(const Grid *grid, float progress)
{
    leds.clear();

    const int eol = grid->eolCount();
    if (eol == 0)
    {
        cellSat = sat;
    }
    else
    {
        const float eolFract = float(eol) / float(grid->EOL_END);
        cellSat = uint8_t((1.0 - eolFract) * float(sat));
    }

    // Detect a new game, and increment the hue.
    if (grid->era() == 0)
    {
        const uint32_t nextHueIndex = grid->resets() % NUM_CELL_HUES;
        cellHue = cellHueOptions[nextHueIndex];
    }

    for (int i = 0; i < ROWS; i++)
    {
        int noff = GRID_ROW_OFFSET[i];
        int rowLen = GRID_ROW_SIZE[i];

        uint32_t cellColor = hsv(cellHue, cellSat, val);
        uint32_t spawnColor = calculateSpawnColor(progress);
        uint32_t killColor = calculateKillColor(progress);

        for (int j = 0; j < rowLen; j++)
        {
            int n = noff + j * 2;
            if (grid->kill[i][j])
            {
                leds.setPixelColor(n, killColor);
            }
            else if (grid->spawn[i][j])
            {
                leds.setPixelColor(n, spawnColor);
            }
            else if (grid->cells[i][j])
            {
                leds.setPixelColor(n, cellColor);
            }
        }
    }
    leds.show();
}

uint32_t LedView::calculateKillColor(float progress)
{
    if (progress < 0.125) // Wait period.
    {
        return hsv(cellHue, cellSat, val);
    }
    else if (progress > 0.75) // Fade out.
    {
        const float a = 1.0 - ((progress - 0.75) * 4.0);
        uint8_t killVal = uint8_t(float(val) * a);
        return hsv(killHue, sat, killVal);
    }
    return hsv(killHue, sat, val);
}

uint32_t LedView::calculateSpawnColor(float progress)
{
    if (progress < 0.25) // Fade in.
    {
        const float a = progress * 4.0;
        uint8_t spawnVal = uint8_t(float(val) * a);
        return hsv(spawnHue, sat, spawnVal);
    }
    else if (progress > 0.875) // Transition.
    {
        return hsv(cellHue, cellSat, val);
    }
    return hsv(spawnHue, sat, val);
    ;
}
