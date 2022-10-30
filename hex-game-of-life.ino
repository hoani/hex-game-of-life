#include "grid.h"
#include "serial_grid.h"
#include "led_grid.h"
#include "keypad.h"

#define LED_GRID_PIN 6

Grid grid;
LedGrid ledGrid = LedGrid(LED_GRID_PIN);
SerialGrid serialGrid;
Keypad keypad;

void setup()
{
  int seed = analogRead(A3);
  seed += millis();
  grid = Grid(seed);
}

void loop()
{
  int key = keypad.sample();
  if (key != -1 && key < 9)
  {
    grid.selectForEdit(key);
    serialGrid.update(&grid);
    ledGrid.update(&grid);
    while (keypad.sample() != -1)
    {
      delay(10);
    }
    delay(100);
    while (true)
    {
      key = keypad.sample();
      if (key != -1)
      {
        if (key < 9)
        {
          grid.applyEdit(key);
        }
        else
        {
          grid.clearEdit();
        }
        break;
      }
    }
  }
  serialGrid.update(&grid);
  ledGrid.update(&grid);

  delay(1500);
  grid.calculateEra();

  serialGrid.update(&grid);
  ledGrid.update(&grid);

  delay(500);
  grid.applyEra();
}
