#include "update.h"

void Update::setSpeed(int multiplier)
{
    _multiplier = multiplier;
}

void Update::update()
{
    int delta = ((int)millis() * _multiplier - _lastUpdateMs);
    if (state == GridState::Idle && delta > idleMs)
    {
        _lastUpdateMs += idleMs;
        _grid->calculateEra();

        _updateView();
        state = GridState::Pending;
    }
    else if (state == GridState::Pending && delta > pendingMs)
    {
        _lastUpdateMs += pendingMs;
        _grid->applyEra();

        _updateView();
        state = GridState::Idle;
    }
    else
    {
        delay(10);
    }
}

void Update::idle()
{
    state = GridState::Idle;
    _lastUpdateMs = millis() * _multiplier;
    _updateView();
}

void Update::_updateView()
{
    _view->update(_grid);
}