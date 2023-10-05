#include "grid.h"
#include "serial_grid.h"
#include "led_grid.h"

#define LED_GRID_PIN 6

Grid grid;
LedGrid ledGrid = LedGrid(LED_GRID_PIN);
SerialGrid serialGrid(Serial1);

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
  seed = millis();
  grid = Grid(seed);
  // keypad = Keypad();
  Serial1.begin(115200);
  update.idle();
}

void loop()
{
  delay(1000);
  Serial1.println("Hello");
  gameOfLifeUpdate();
}

void gameOfLifeUpdate()
{
  update.update();
}
