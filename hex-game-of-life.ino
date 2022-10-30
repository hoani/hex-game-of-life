#include "grid.h"
#include "serial_grid.h"
#include "led_grid.h"

#define LED_GRID_PIN 6

Grid grid;
LedGrid ledGrid = LedGrid(LED_GRID_PIN);
SerialGrid serialGrid;

void setup()
{
  int seed = analogRead(A3);
  seed += millis();
  grid = Grid(seed);
}

void loop()
{
  serialGrid.update(&grid);
  ledGrid.update(&grid);

  delay(1500);
  grid.calculateEra();

  serialGrid.update(&grid);
  ledGrid.update(&grid);

  delay(500);
  grid.applyEra();
}
