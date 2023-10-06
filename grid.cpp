#include "grid.h"
#include "arduino.h"

// Without a seed, we create a grid with a "symmetric" pattern. This is for testing end of life.
Grid::Grid()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (j < rowLength(i))
            {
                cells[i][j] = (((i - 4) % 2) == 0) || (((j - 4) % 2) == 0);
            }
            else
            {
                cells[i][j] = false;
            }
            kill[i][j] = false;
            spawn[i][j] = false;
        }
    }
}

Grid::Grid(int seed)
{
    randomSeed(seed);
}

void Grid::calculateEra()
{
    const uint64_t currentEol = _currentEol();

    // Recovery - this is usually a result of end of life destroying the board.
    if (currentEol == 0x0)
    {
        reset();
        return;
    }

    const bool gridFull = (currentEol == EOL_GRID_FULL);
    const bool doEol = (_eolCount >= EOL_DELAY);

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (doEol == false)
            {
                _calculateCell(i, j);
            }
            else
            {
                // End of life: We overpopulate the board.
                // Once the board is full, kill all cells.
                if (gridFull == false)
                {
                    _calculateEolCell(i, j);
                }
                else
                {
                    kill[i][j] = true;
                }
            }
        }
    }
}

void Grid::applyEra()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            _applyCell(i, j);
        }
    }
    _updateEol(); // Do end of life detection.
}

int Grid::eolCount() const
{
    return _eolCount;
}

void Grid::reset()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (j < rowLength(i))
            {
                spawn[i][j] = (random(2) == 1 && cellExists(i, j));
            }
            else
            {
                spawn[i][j] = false;
            }
            kill[i][j] = false;
            cells[i][j] = false;
        }
    }

    _eolNext = 0;
    _eolCount = 0;
    for (int i = 0; i < EOL_DETECT_LEN; i++)
    {
        _eolEntries[i] = 0;
    }
}

// Private after here.

void Grid::_applyCell(int i, int j)
{
    if (kill[i][j])
    {
        cells[i][j] = false;
        kill[i][j] = false;
    }
    if (spawn[i][j])
    {
        cells[i][j] = true;
        spawn[i][j] = false;
    }
}

void Grid::_calculateCell(int i, int j)
{
    if (cellExists(i, j) == false)
    {
        return;
    }
    int count = _countNeighbours(i, j);
    if (cells[i][j])
    {
        if (count < 2 || count > 3)
        {
            kill[i][j] = true;
        }
    }
    else
    {
        if (count == 2 || count == 3)
        {
            spawn[i][j] = true;
        }
    }
}

void Grid::_calculateEolCell(int i, int j)
{
    if (cellExists(i, j) == false)
    {
        return;
    }
    int count = _countNeighbours(i, j);
    if (!cells[i][j])
    {
        if (count > 1)
        {
            spawn[i][j] = true;
        }
    }
}

int Grid::_countNeighbours(int i, int j)
{
    int rowLen = rowLength(i);
    int count = 0;
    // prev row
    if (i > 0)
    {
        int prevLen = rowLength(i - 1);
        if (prevLen < rowLen)
        {
            count += _neighboursFromSmallerRow(i - 1, j, prevLen);
        }
        else
        {
            count += _neighboursFromLargerRow(i - 1, j);
        }
    }
    // cell row
    if (j > 0)
    {
        if (cells[i][j - 1])
        {
            count++;
        }
    }
    if (j < rowLen - 1)
    {
        if (cells[i][j + 1])
        {
            count++;
        }
    }

    // next row
    if (i < (ROWS - 1))
    {
        int nextLen = rowLength(i + 1);
        if (nextLen < rowLen)
        {
            count += _neighboursFromSmallerRow(i + 1, j, nextLen);
        }
        else
        {
            count += _neighboursFromLargerRow(i + 1, j);
        }
    }
    return count;
}

int Grid::_neighboursFromSmallerRow(int i, int j, int len)
{
    int count = 0;
    if (j > 0)
    {
        if (cells[i][j - 1])
        {
            count++;
        }
    }
    if (j < len)
    {
        if (cells[i][j])
        {
            count++;
        }
    }
    return count;
}

int Grid::_neighboursFromLargerRow(int i, int j)
{
    int count = 0;
    if (cells[i][j])
    {
        count++;
    }
    if (cells[i][j + 1])
    {
        count++;
    }
    return count;
}

bool Grid::cellExists(int i, int j)
{
    // Shouldn't happen, but just incase
    if (i < 0 || j < 0 || i >= ROWS || j >= COLS)
    {
        return false;
    }
    // handle shorter rows.
    if (j >= rowLength(i))
    {
        return false;
    }
    return true;
}

int Grid::rowLength(int i)
{
    return GRID_ROW_SIZE[i];
}

void Grid::_updateEol()
{
    if (_eolCount != 0)
    {
        _eolCount++; // Already at end of life. Just increment and return.
        return;
    }

    uint64_t entry = _currentEol();
    // Check if current entry matches existing entries.
    for (int i = 0; i < EOL_DETECT_LEN; i++)
    {
        if (entry == _eolEntries[i])
        {
            _eolCount += 1;
            return;
        }
    }
    // Record the entry since we didn't find a match.
    _eolEntries[_eolNext] = entry;
    _eolNext = (_eolNext + 1) % EOL_DETECT_LEN;
}

uint64_t Grid::_currentEol()
{
    uint64_t entry = 0;
    int index = 0;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (cellExists(i, j))
            {
                if (cells[i][j])
                {
                    entry |= ((uint64_t)0x1 << index);
                }
                index++;
            }
        }
    }
    return entry;
}
