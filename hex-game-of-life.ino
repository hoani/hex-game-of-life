#include "grid.h"
#include "serial_grid.h"
#include "led_grid.h"
#include "update.h"

#define LED_GRID_PIN 6

Grid grid;
LedGrid ledGrid = LedGrid(LED_GRID_PIN);
SerialGrid serialGrid(Serial1);

void setup()
{
  int seed = analogRead(A3);
  seed = millis();
  grid = Grid(seed);
  Serial1.begin(115200);

  CompoundGridView view(ledGrid, serialGrid);

  GridUpdate update(&grid, &view);
  update.idle();
  while (true)
  {
    delay(1000);
    update.update();
  }
}

void loop()
{
  delay(1000);
}
