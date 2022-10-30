#include "serial_grid.h"
#include "arduino.h"

SerialGrid::SerialGrid()
{
    Serial.begin(115200);
}

void SerialGrid::update(const Grid *grid)
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
            if (grid->kill[i][j])
            {
                Serial.print("-");
            }
            else if (grid->spawn[i][j])
            {
                Serial.print("+");
            }
            else if (grid->cells[i][j])
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
