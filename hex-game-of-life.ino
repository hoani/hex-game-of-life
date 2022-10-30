#include "grid.h"
#include "serial_grid.h"
#include "led_grid.h"
#include "keypad.h"

#define LED_GRID_PIN 6

Grid grid;
LedGrid ledGrid = LedGrid(LED_GRID_PIN);
SerialGrid serialGrid;
Keypad keypad;

class GridUpdate
{
public:
  GridUpdate() : lastUpdateMs(millis()) {}

  void update()
  {
    int delta = (int)millis() - lastUpdateMs;
    if (state == GridState::Idle && delta > idleMs)
    {
      lastUpdateMs += idleMs;
      grid.calculateEra();

      serialGrid.update(&grid);
      ledGrid.update(&grid);
      state = GridState::Pending;
    }
    else if (state == GridState::Pending && delta > pendingMs)
    {
      lastUpdateMs += pendingMs;
      grid.applyEra();

      serialGrid.update(&grid);
      ledGrid.update(&grid);
      state = GridState::Idle;
    }
  }

  void idle()
  {
    state = GridState::Idle;
    lastUpdateMs = millis();
    serialGrid.update(&grid);
    ledGrid.update(&grid);
  }

  enum class GridState
  {
    Idle,
    Pending,
  };
  GridState state = GridState::Idle;

private:
  int lastUpdateMs;

  const int idleMs = 1500;
  const int pendingMs = 500;
};

GridUpdate update;

void gameOfLifeUpdate();

void setup()
{
  int seed = analogRead(A3);
  seed += millis();
  grid = Grid(seed);
  keypad = Keypad();
  update.idle();
}

void loop()
{
  delay(10);
  gameOfLifeUpdate();
}

void gameOfLifeUpdate()
{
  int key = keypad.sample();
  if (key != -1 && key < 9)
  {
    update.idle();
    grid.selectForEdit(key);
    serialGrid.update(&grid);
    ledGrid.update(&grid);
    while (keypad.sample() != -1)
    {
    }
    int start = millis();
    const int timeout = 10000; // 10 seconds.
    while ((millis() - start) < timeout)
    {
      key = keypad.sample();
      if (key != -1)
      {
        if (key < 9)
        {
          grid.applyEdit(key);
        }
        break;
      }
    }
    grid.clearEdit();
    update.idle();
    while (keypad.sample() != -1)
    {
    }
  }
  update.update();
}
