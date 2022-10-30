#ifndef _HGOL_SERIAL_GRID_H_
#define _HGOL_SERIAL_GRID_H_

#include "grid.h"

class SerialGrid
{
public:
    SerialGrid();

    void update(const Grid *grid);
};

#endif