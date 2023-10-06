#ifndef _HGOL_UPDATE_H_
#define _HGOL_UPDATE_H_

#include "grid.h"
#include "arduino.h"

enum class GridState
{
    Idle,
    Pending,
};

class Update
{
public:
    Update(Grid *grid, GridView *view) : _lastUpdateMs(millis()), _grid(grid), _view(view) {}

    void setSpeed(int multiplier);
    void update();
    void idle();

    GridState state = GridState::Idle;

private:
    int _lastUpdateMs;
    GridView *_view;
    Grid *_grid;
    int _multiplier = 1;

    const int idleMs = 1500;
    const int pendingMs = 500;

    void _updateView();
};

#endif /* _HGOL_UPDATE_H_ */
