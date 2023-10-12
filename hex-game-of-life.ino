#include "grid.h"
#include "audio_view.h"
#include "serial_view.h"
#include "led_view.h"
#include "update.h"

#define LED_GRID_PIN 1

void setup()
{
  int seed = analogRead(A3) + analogRead(A4);
  seed += millis();
  // Grid grid(seed);
  Grid grid; // For debugging only.

  Serial.begin(115200);

  LedView ledView(LED_GRID_PIN);
  AudioView audioView;
  CompoundGridView hwView(ledView, audioView);

  SerialView serialView(Serial);
  CompoundGridView view(hwView, serialView);
  // CompoundGridView view(audioView, serialView);

  Update update(&grid, &view);
  update.setSpeed(1);
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
