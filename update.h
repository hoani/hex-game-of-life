#ifndef _HGOL_UPDATE_H_
#define _HGOL_UPDATE_H_

#include "grid.h"
#include "arduino.h"

class GridUpdate
{
public:
    GridUpdate() {}
    GridUpdate(Grid *grid, GridView *view) : _lastUpdateMs(millis()), _grid(grid), _view(view) {}

    void update()
    {
        int delta = (int)millis() - _lastUpdateMs;
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
    }

    void idle()
    {
        state = GridState::Idle;
        _lastUpdateMs = millis();
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

    const int idleMs = 1500;
    const int pendingMs = 500;
};

#endif /* _HGOL_UPDATE_H_ */
