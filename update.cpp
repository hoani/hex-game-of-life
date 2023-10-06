#include "update.h"

void Update::setSpeed(int multiplier)
{
    _multiplier = multiplier;
}

void Update::update()
{
    int delta = ((int)millis() * _multiplier - _lastUpdateMs);
    float progress = 1.0;
    switch (state)
    {
    case GridState::Idle:
        progress = float(delta) / float(idleMs);
        if (progress > 1.0)
        {
            progress = 1.0;
            _lastUpdateMs += idleMs;
            _updateView(progress);
            _grid->calculateEra();

            state = GridState::Pending;
            return;
        }
        break;
    case GridState::Pending:
        progress = float(delta) / float(pendingMs);
        if (progress > 1.0)
        {
            progress = 1.0;
            _lastUpdateMs += pendingMs;
            _updateView(progress);
            _grid->applyEra();

            state = GridState::Idle;
            return;
        }
        break;
    }

    _updateView(progress);
    delay(5);
}

void Update::idle()
{
    state = GridState::Idle;
    _lastUpdateMs = millis() * _multiplier;
    _updateView(1.0);
}

void Update::_updateView(float progress)
{
    _view->update(_grid, progress);
}