#include "grid.h"
#include "arduino.h"

Grid::Grid() : Grid(millis()) {}

Grid::Grid(int seed)
{
    randomSeed(seed);
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            cells[i][j] = (random(2) == 1 && cellExists(i, j));
            kill[i][j] = false;
            spawn[i][j] = false;
            edit[i][j] = false;
        }
    }
}

void Grid::selectForEdit(int k)
{
    for (int i = 0; i < ROWS; i++)
    {
        int j;
        if (i >= SIZE)
        {
            j = COLS - (k + 1);
        }
        else
        {
            j = COLS - (k + SIZE - i);
        }
        if (j < 0 || j >= rowLength(i))
        {
            continue;
        }
        edit[i][j] = true;
    }
    editIndex = k;
}

void Grid::applyEdit(int k)
{
    int iCurrent = rowLength(editIndex);
    for (int i = 0; i < ROWS; i++)
    {
        int j;
        if (i >= SIZE)
        {
            j = COLS - (editIndex + 1);
        }
        else
        {
            j = COLS - (editIndex + SIZE - i);
        }
        if (j < 0 || j >= rowLength(i))
        {
            continue;
        }
        iCurrent--; // A bit convoluted, but we have to count backwards.
        if (iCurrent == k)
        {
            cells[i][j] = !cells[i][j];
            kill[i][j] = false;
            spawn[i][j] = false;
            break;
        }
    }
    clearEdit();
}

void Grid::clearEdit()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            edit[i][j] = false;
        }
    }
}

void Grid::calculateEra()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            calculateCell(i, j);
        }
    }
}

void Grid::applyEra()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            applyCell(i, j);
        }
    }
}

// Private after here.

void Grid::applyCell(int i, int j)
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

void Grid::calculateCell(int i, int j)
{
    if (cellExists(i, j) == false)
    {
        return;
    }
    int count = countNeighbours(i, j);
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

int Grid::countNeighbours(int i, int j)
{
    int rowLen = rowLength(i);
    int count = 0;
    // prev row
    if (i > 0)
    {
        int prevLen = rowLength(i - 1);
        if (prevLen < rowLen)
        {
            count += neighboursFromSmallerRow(i - 1, j, prevLen);
        }
        else
        {
            count += neighboursFromLargerRow(i - 1, j);
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
    if (j < COLS - 1)
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
            count += neighboursFromSmallerRow(i + 1, j, nextLen);
        }
        else
        {
            count += neighboursFromLargerRow(i + 1, j);
        }
    }
    return count;
}

int Grid::neighboursFromSmallerRow(int i, int j, int len)
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

int Grid::neighboursFromLargerRow(int i, int j)
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
    if (j > rowLength(i))
    {
        return false;
    }
    return true;
}

int Grid::rowLength(int i)
{
    return GRID_ROW_SIZE[i];
}