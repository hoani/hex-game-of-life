#ifndef _HGOL_UPDATE_H_
#define _HGOL_UPDATE_H_

#include "grid.h"
#include "arduino.h"

class GridUpdate
{
public:
    GridUpdate() {}
    GridUpdate(Grid *grid, GridView *view) : _lastUpdateMs(millis()), _grid(grid), _view(view) {}

    void setSpeed(int multiplier)
    {
        _multiplier = multiplier;
    }

    void update()
    {
        int delta = ((int)millis() * _multiplier - _lastUpdateMs);
        if (state == GridState::Idle && delta > idleMs)
        {
            _lastUpdateMs += idleMs;
            _grid->calculateEra();

            viewUpdate();
            state = GridState::Pending;
        }
        else if (state == GridState::Pending && delta > pendingMs)
        {
            _lastUpdateMs += pendingMs;
            _grid->applyEra();

            viewUpdate();
            state = GridState::Idle;
        }
        else
        {
            delay(10);
        }
    }

    void idle()
    {
        state = GridState::Idle;
        _lastUpdateMs = millis() * _multiplier;
        viewUpdate();
    }

    void viewUpdate()
    {
        _view->update(_grid);
    }

    enum class GridState
    {
        Idle,
        Pending,
    };
    GridState state = GridState::Idle;

private:
    int _lastUpdateMs;
    GridView *_view;
    Grid *_grid;
    int _multiplier = 1;

    const int idleMs = 1500;
    const int pendingMs = 500;
};

#endif /* _HGOL_UPDATE_H_ */
