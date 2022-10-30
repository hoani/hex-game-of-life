#ifndef _HGOL_GRID_H_
#define _HGOL_GRID_H_

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

private:
    void calculateCell(int i, int j);
    void applyCell(int i, int j);
    int countNeighbours(int i, int j);
    int neighboursFromSmallerRow(int i, int j, int len);
    int neighboursFromLargerRow(int i, int j);
};

#endif