#include "board.hpp"

#include <cassert>
#include <cstring>

Board::Board(int size, bool initial) : boardSize(size)
{
    // Allocate 2D array
    grid = new Disk*[boardSize];
    for (int i = 0; i < boardSize; i++) {
        grid[i] = new Disk[boardSize];
    }

    for (int y = 0; y < boardSize; y++)
        for (int x = 0; x < boardSize; x++)
            grid[y][x] = Disk::Empty;

    if (initial)
        reset();
}

Board::~Board()
{
    for (int i = 0; i < boardSize; i++) {
        delete[] grid[i];
    }
    delete[] grid;
}

void Board::reset()
{
    for (int y = 0; y < boardSize; y++)
        for (int x = 0; x < boardSize; x++)
            grid[y][x] = Disk::Empty;

    // Set initial pieces in center
    int center = boardSize / 2;
    grid[center-1][center-1] = Disk::O;
    grid[center-1][center] = Disk::X;
    grid[center][center-1] = Disk::X;
    grid[center][center] = Disk::O;
}

Board::Disk Board::get(int x, int y) const
{
    return grid[y][x];
}

bool Board::scan(int startX, int startY, int dx, int dy, Disk current) const
{
    int x = startX + dx;
    int y = startY + dy;
    bool anyOther = false;

    while (x >= 0 && x < boardSize && y >= 0 && y < boardSize) {
        if (grid[y][x] == Disk::Empty)
            return false;

        if (grid[y][x] == current)
            return anyOther;

        anyOther = true;
        x += dx;
        y += dy;
    }

    return false;
}

bool Board::isValid(int x, int y, Disk current) const
{
    if (grid[y][x] != Disk::Empty)
        return false;

    return scan(x, y, 0, 1, current) ||
           scan(x, y, 1, 0, current) ||
           scan(x, y, 0, -1, current) ||
           scan(x, y, -1, 0, current) ||
           scan(x, y, 1, 1, current) ||
           scan(x, y, 1, -1, current) ||
           scan(x, y, -1, 1, current) ||
           scan(x, y, -1, -1, current);
}

std::vector<std::pair<int, int>> Board::getValid(Disk current) const
{
    std::vector<std::pair<int, int>> out;
    for (int y = 0; y < boardSize; y++) {
        for (int x = 0; x < boardSize; x++) {
            if (isValid(x, y, current))
                out.emplace_back(x, y);
        }
    }
    return out;
}

void Board::scanAndFlip(int startX, int startY, int dx, int dy)
{
    int x = startX;
    int y = startY;

    bool anyOther = false;
    Disk border = grid[y][x];

    while (true) {
        x += dx;
        y += dy;
        if (x < 0 || x >= boardSize || y < 0 || y >= boardSize)
            return;
        if (grid[y][x] == Disk::Empty)
            return;
        if (grid[y][x] == border) {
            if (!anyOther)
                return;
            // flip back towards start
            int fx = startX + dx;
            int fy = startY + dy;
            while (fx != x || fy != y) {
                grid[fy][fx] = border;
                fx += dx;
                fy += dy;
            }
            return;
        }
        anyOther = true;
    }
}

void Board::put(int x, int y, Disk current)
{
    assert(grid[y][x] == Disk::Empty);
    grid[y][x] = current;

    scanAndFlip(x, y, 0, 1);
    scanAndFlip(x, y, 1, 0);
    scanAndFlip(x, y, 0, -1);
    scanAndFlip(x, y, -1, 0);
    scanAndFlip(x, y, 1, 1);
    scanAndFlip(x, y, 1, -1);
    scanAndFlip(x, y, -1, 1);
    scanAndFlip(x, y, -1, -1);
}

int Board::count(Disk who) const
{
    int c = 0;
    for (int y = 0; y < boardSize; y++)
        for (int x = 0; x < boardSize; x++)
            if (grid[y][x] == who)
                c++;
    return c;
}


