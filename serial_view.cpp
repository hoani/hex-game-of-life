#include "serial_view.h"

void SerialView::update(const Grid *grid, float progress)
{
    if (progress != 1.0)
    {
        return;
    }
    _serial.println("-----------------");
    for (int i = 0; i < ROWS; i++)
    {
        int rowLen = Grid::rowLength(i);

        _serial.print("i=");
        _serial.print(i);
        _serial.print("\t.");
        int empties = (COLS - rowLen);
        for (int k = 0; k < empties; k++)
        {
            _serial.print(".");
        }
        for (int j = 0; j < rowLen; j++)
        {
            if (grid->kill[i][j])
            {
                _serial.print("-");
            }
            else if (grid->spawn[i][j])
            {
                _serial.print("+");
            }
            else if (grid->cells[i][j])
            {
                _serial.print("O");
            }
            else
            {
                _serial.print(" ");
            }
            _serial.print(".");
        }
        for (int k = 0; k < empties; k++)
        {
            _serial.print(".");
        }
        _serial.println("");
    }
    _serial.println("-----------------");
    _serial.print("Resets: ");
    _serial.print(grid->resets());
    _serial.print("\tEOL: ");
    _serial.print(grid->eolCount());
    _serial.print("\tERA: ");
    _serial.print(grid->era());
    _serial.println("");
    _serial.println("-----------------");
}
