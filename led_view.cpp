#include "led_view.h"

const int NUM_GRID_PIXELS = 113;

const int GRID_ROW_OFFSET[ROWS] = {0, 9, 20, 33, 48, 65, 80, 93, 104};

LedView::LedView(int pin)
{
    pixels = Adafruit_NeoPixel(NUM_GRID_PIXELS, pin, NEO_GRB + NEO_KHZ800);
    pixels.begin();
}

void LedView::update(const Grid *grid)
{
    pixels.clear();

    for (int i = 0; i < ROWS; i++)
    {
        int noff = GRID_ROW_OFFSET[i];
        int rowLen = GRID_ROW_SIZE[i];

        uint32_t cellColor = pixels.gamma32(pixels.ColorHSV(cellHue, sat, val));
        uint32_t spawnColor = pixels.gamma32(pixels.ColorHSV(spawnHue, sat, val));
        uint32_t killColor = pixels.gamma32(pixels.ColorHSV(killHue, sat, val));

        for (int j = 0; j < rowLen; j++)
        {
            int n = noff + j * 2;
            if (grid->kill[i][j])
            {
                pixels.setPixelColor(n, killColor);
            }
            else if (grid->spawn[i][j])
            {
                pixels.setPixelColor(n, spawnColor);
            }
            else if (grid->cells[i][j])
            {
                pixels.setPixelColor(n, cellColor);
            }
        }
    }
    pixels.show();
}
