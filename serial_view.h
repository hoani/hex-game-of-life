#ifndef _HGOL_SERIAL_VIEW_H_
#define _HGOL_SERIAL_VIEW_H_

#include "grid.h"
#include "arduino.h"

class SerialView : public GridView
{
public:
    SerialView(Stream &ser) : _serial(ser) {}

    void update(const Grid *grid);

private:
    Stream &_serial;
};

#endif