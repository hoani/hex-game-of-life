#include "led_grid.h"

Grid grid;

const int NUM_GRID_PIXELS = 113;

const int GRID_ROW_OFFSET[ROWS] = {0, 9, 20, 33, 48, 65, 80, 93, 104};

Adafruit_NeoPixel gridPixels(NUM_GRID_PIXELS, GRID_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  int seed = analogRead(A3);
  seed += millis();
  grid = Grid(seed);

  Serial.begin(115200);

  // Init led grid
  gridPixels.begin();
}

void loop()
{
  delay(1500);

  grid.calculateEra();

  printCells();
  pixelCells();
  delay(500);

  grid.applyEra();

  printCells();
  pixelCells();
}

void pixelCells()
{
  gridPixels.clear();
  for (int i = 0; i < ROWS; i++)
  {
    int noff = GRID_ROW_OFFSET[i];
    int rowLen = GRID_ROW_SIZE[i];

    for (int j = 0; j < rowLen; j++)
    {
      int n = noff + j * 2;
      if (grid.kill[i][j])
      {
        gridPixels.setPixelColor(n, 100, 0, 0);
      }
      else if (grid.spawn[i][j])
      {
        gridPixels.setPixelColor(n, 0, 100, 0);
      }
      else if (grid.cells[i][j])
      {
        gridPixels.setPixelColor(n, 0, 100, 200);
      }
    }
  }
  gridPixels.show();
}

void printCells()
{
  Serial.println("-----------------");
  for (int i = 0; i < ROWS; i++)
  {
    int rowLen = Grid::rowLength(i);

    Serial.print("i=");
    Serial.print(i);
    Serial.print("\t.");
    int empties = (COLS - rowLen);
    for (int k = 0; k < empties; k++)
    {
      Serial.print(".");
    }
    for (int j = 0; j < rowLen; j++)
    {
      if (grid.kill[i][j])
      {
        Serial.print("-");
      }
      else if (grid.spawn[i][j])
      {
        Serial.print("+");
      }
      else if (grid.cells[i][j])
      {
        Serial.print("O");
      }
      else
      {
        Serial.print(" ");
      }
      Serial.print(".");
    }
    for (int k = 0; k < empties; k++)
    {
      Serial.print(".");
    }
    Serial.println("");
  }
  Serial.println("-----------------");
}
