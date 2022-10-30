#include "led_grid.h"

const int NUM_GRID_PIXELS = 113;

const int GRID_ROW_OFFSET[ROWS] = {0, 9, 20, 33, 48, 65, 80, 93, 104};

LedGrid::LedGrid(int pin)
{
    pixels = Adafruit_NeoPixel(NUM_GRID_PIXELS, pin, NEO_GRB + NEO_KHZ800);
    pixels.begin();
}

void LedGrid::update(const Grid *grid)
{
    pixels.clear();
    for (int i = 0; i < ROWS; i++)
    {
        int noff = GRID_ROW_OFFSET[i];
        int rowLen = GRID_ROW_SIZE[i];

        for (int j = 0; j < rowLen; j++)
        {
            int n = noff + j * 2;
            if (grid->edit[i][j])
            {
                if (grid->cells[i][j])
                {
                    pixels.setPixelColor(n, 50, 20, 50);
                }
                else
                {
                    pixels.setPixelColor(n, 20, 50, 50);
                }
            }
            else if (grid->kill[i][j])
            {
                pixels.setPixelColor(n, 100, 0, 0);
            }
            else if (grid->spawn[i][j])
            {
                pixels.setPixelColor(n, 0, 100, 0);
            }
            else if (grid->cells[i][j])
            {
                pixels.setPixelColor(n, 0, 100, 200);
            }
        }
    }
    pixels.show();
}
