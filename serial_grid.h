#ifndef _HGOL_SERIAL_GRID_H_
#define _HGOL_SERIAL_GRID_H_

#include "grid.h"
#include "arduino.h"

class SerialGrid
{
public:
    SerialGrid(Stream &ser) :_serial(ser){}

    void update(const Grid *grid);
private:
    Stream &_serial;
};

#endif