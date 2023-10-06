#include "led_view.h"

const int NUM_GRID_PIXELS = 113;

const int GRID_ROW_OFFSET[ROWS] = {0, 9, 20, 33, 48, 65, 80, 93, 104};

LedView::LedView(int pin)
{
    pixels = Adafruit_NeoPixel(NUM_GRID_PIXELS, pin, NEO_GRB + NEO_KHZ800);
    pixels.begin();
}

void LedView::update(const Grid *grid, float progress)
{
    pixels.clear();

    const int eol = grid->eolCount();
    if (eol == 0)
    {
        cellSat = sat;
    }
    else
    {
        const float eolFract = float(eol) / float(grid->EOL_DELAY);
        cellSat = uint8_t((1.0 - eolFract) * float(sat));
    }

    for (int i = 0; i < ROWS; i++)
    {
        int noff = GRID_ROW_OFFSET[i];
        int rowLen = GRID_ROW_SIZE[i];

        uint32_t cellColor = pixels.gamma32(pixels.ColorHSV(cellHue, cellSat, val));
        uint32_t spawnColor = calculateSpawnColor(progress);
        uint32_t killColor = calculateKillColor(progress);

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

uint32_t LedView::calculateKillColor(float progress)
{
    if (progress < 0.125) // Wait period.
    {
        return pixels.gamma32(pixels.ColorHSV(cellHue, cellSat, val));
    }
    else if (progress > 0.75) // Fade out.
    {
        const float a = 1.0 - ((progress - 0.75) * 4.0);
        uint8_t killVal = uint8_t(float(val) * a);
        return pixels.gamma32(pixels.ColorHSV(killHue, sat, killVal));
    }
    return pixels.gamma32(pixels.ColorHSV(killHue, sat, val));
}

uint32_t LedView::calculateSpawnColor(float progress)
{
    if (progress < 0.25) // Fade in.
    {
        const float a = progress * 4.0;
        uint8_t spawnVal = uint8_t(float(val) * a);
        return pixels.gamma32(pixels.ColorHSV(spawnHue, sat, spawnVal));
    }
    else if (progress > 0.875) // Transition.
    {
        return pixels.gamma32(pixels.ColorHSV(cellHue, cellSat, val));
    }
    return pixels.gamma32(pixels.ColorHSV(spawnHue, sat, val));
    ;
}
