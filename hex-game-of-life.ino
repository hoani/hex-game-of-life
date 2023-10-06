#include "grid.h"
#include "serial_view.h"
#include "led_view.h"
#include "update.h"

#define LED_GRID_PIN 6

void setup()
{
  int seed = analogRead(A3);
  seed = millis();
  // Grid grid(seed);
  Grid grid;

  Serial1.begin(115200);
  SerialView serialView(Serial1);

  LedView ledView(LED_GRID_PIN);

  CompoundGridView view(ledView, serialView);

  GridUpdate update(&grid, &view);
  update.setSpeed(4);
  update.idle();
  while (true)
  {
    update.update();
  }
}

void loop()
{
  delay(1000);
}
