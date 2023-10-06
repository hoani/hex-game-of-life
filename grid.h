#ifndef _HGOL_GRID_H_
#define _HGOL_GRID_H_

#include <stdint.h>

const int SIZE = 5;
const int ROWS = (2 * SIZE - 1);
const int COLS = ROWS;
const int GRID_ROW_SIZE[ROWS] = {5, 6, 7, 8, 9, 8, 7, 6, 5};

class Grid
{
public:
    Grid();
    Grid(int seed);
    bool cells[ROWS][COLS];
    bool kill[ROWS][COLS];
    bool spawn[ROWS][COLS];

    static int rowLength(int i);
    static bool cellExists(int i, int j);

    void calculateEra();
    void applyEra();

    int eolCount() const;

    void reset();

    const static int EOL_DELAY = 128;
    const static uint64_t EOL_GRID_FULL = 0x1fffffffffffffff;
    const static uint64_t EOL_GRID_EMPTY = 0x0;

private:
    void _calculateCell(int i, int j);
    void _calculateEolCell(int i, int j);
    void _applyCell(int i, int j);
    int _countNeighbours(int i, int j);
    int _neighboursFromSmallerRow(int i, int j, int len);
    int _neighboursFromLargerRow(int i, int j);

    // End of life detection.
    const static int EOL_DETECT_LEN = 64;
    int _eolNext = 0;
    uint64_t _eolEntries[EOL_DETECT_LEN] = {0};
    int _eolCount = 0;
    void _updateEol();
    uint64_t _currentEol();
};

class GridView
{
public:
    virtual void update(const Grid *grid, float progress);
};

class CompoundGridView : public GridView
{
public:
    CompoundGridView(GridView &a, GridView &b) : _a(a), _b(b) {}

    void update(const Grid *grid, float progress)
    {
        _a.update(grid, progress);
        _b.update(grid, progress);
    }

private:
    GridView &_a;
    GridView &_b;
};

#endif