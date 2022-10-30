#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define GRID_PIN        6 




const int SIZE = 5;
const int ROWS = (2*SIZE - 1);
const int COLS = ROWS;
bool cells[ROWS][COLS] = {};
bool kill[ROWS][COLS] = {};
bool spawn[ROWS][COLS] = {};

const int NUM_GRID_PIXELS = 113;
const int GRID_ROW_SIZE[ROWS] = {5, 6, 7, 8, 9, 8, 7, 6, 5};
const int GRID_ROW_OFFSET[ROWS] = {0, 9, 20, 33, 48, 65, 80, 93, 104};

Adafruit_NeoPixel gridPixels(NUM_GRID_PIXELS, GRID_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  int seed = analogRead(A3);
  seed += millis();
  randomSeed(seed);
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      cells[i][j] = (random(2) == 1 && cellExists(i, j));
      kill[i][j] = false;
      spawn[i][j] = false;
    }
  }
  Serial.begin(115200);

  // Init led grid
  gridPixels.begin(); 
}

void loop() {
  delay(1500);
  for (int i = 0; i<ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      cellUpdate(i, j);
    }
  }
  
  printCells();
  pixelCells();
  delay(500);
  for (int i = 0; i<ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      applyCellUpdate(i, j);
    }
  }
  printCells();
  pixelCells();
}

void pixelCells() {
  gridPixels.clear(); 
  for (int i = 0; i<ROWS; i++) {
    int noff = GRID_ROW_OFFSET[i];
    int rowLen = GRID_ROW_SIZE[i];
   
    for (int j = 0; j < rowLen; j++) {
      int n = noff + j * 2;
      if (kill[i][j]) {
        gridPixels.setPixelColor(n, 100, 0, 0);
      } else if (spawn[i][j]) {
        gridPixels.setPixelColor(n, 0, 100, 0);
      } else if (cells[i][j]) {
        gridPixels.setPixelColor(n, 0, 100, 200);
      }
    }
  }
  gridPixels.show();
}

void printCells() {
  Serial.println("-----------------");
  for (int i = 0; i<ROWS; i++) {
    int rowLen = rowLength(i);
    
    Serial.print("i=");
    Serial.print(i);
    Serial.print("\t.");
    int empties = (COLS - rowLen);
    for (int k = 0; k < empties; k++) {
      Serial.print(".");
    }
    for (int j = 0; j < rowLen; j++) {
      if (kill[i][j]) {
        Serial.print("-");
      } else if (spawn[i][j]) {
        Serial.print("+");
      } else if (cells[i][j]) {
        Serial.print("O");
      } else {
        Serial.print(" ");
      }
      Serial.print(".");
    }
    for (int k = 0; k < empties; k++) {
      Serial.print(".");
    }
    Serial.println("");
  }
  Serial.println("-----------------");
}

void applyCellUpdate(int i, int j) {
  if (kill[i][j]) {
    cells[i][j] = false;
    kill[i][j] = false;
  }
  if (spawn[i][j]) {
    cells[i][j] = true;
    spawn[i][j] = false;
  }
}

void cellUpdate(int i, int j) {
  if (cellExists(i, j) == false) {
    return;
  }
  int count = countNeighbours(i, j);
  if (cells[i][j]) {
    if (count < 2 || count > 3) {
      kill[i][j] = true;
    }
  } else {
    if (count == 2 || count==3) {
      spawn[i][j] = true;
    }
  }
}

int countNeighbours(int i, int j) {
  int rowLen = rowLength(i);
  int count = 0;
  // prev row
  if (i > 0) {
    int prevLen = rowLength(i-1);
    if (prevLen < rowLen) {
      count += neighboursFromSmallerRow(i-1, j, prevLen);
    } else {
      count += neighboursFromLargerRow(i-1, j);
    }
  }
  // cell row
  if (j > 0) {
    if (cells[i][j-1]) {
      count++;
    }
  }
  if (j < COLS -1) {
    if (cells[i][j+1]) {
      count++;
    }
  }

  // next row
  if (i < (ROWS -1)) {
    int nextLen = rowLength(i+1);
    if (nextLen < rowLen) {
      count += neighboursFromSmallerRow(i+1, j, nextLen);
    } else {
      count += neighboursFromLargerRow(i+1, j);
    }
  }
  return count;
}

int neighboursFromSmallerRow(int i, int j, int len) {
  int count = 0;
  if (j > 0) {
    if (cells[i][j-1]) {
      count++;
    }
  }
  if (j < len) {
    if (cells[i][j]) {
      count++;
    }
  }
  return count;
}

int neighboursFromLargerRow(int i, int j) {
  int count = 0;
  if (cells[i][j]) {
    count++;
  }
  if (cells[i][j+1]) {
    count++;
  }
  return count;
}

bool cellExists(int i, int j) {
  // Shouldn't happen, but just incase 
  if (i < 0 || j < 0 || i >= ROWS || j >= COLS) {
    return false;
  }
  // handle shorter rows.
  if (j > rowLength(i)) {
    return false;
  }
  return true;
}

int rowLength(int i) {
  return GRID_ROW_SIZE[i];
}
