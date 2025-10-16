#pragma once

#include <string>
#include <utility>
#include <vector>

class Board {
public:
    enum class Disk { Empty = 0, X, O };

private:
    int boardSize;
    Disk** grid;

    bool scan(int startX, int startY, int dx, int dy, Disk current) const;
    void scanAndFlip(int startX, int startY, int dx, int dy);

public:
    Board(int size = 8, bool initial = true);
    ~Board();
    void reset();
    int getSize() const { return boardSize; }

    bool isValid(int x, int y, Disk current) const;
    std::vector<std::pair<int, int>> getValid(Disk current) const;
    void put(int x, int y, Disk current);
    Disk get(int x, int y) const;

    int count(Disk who) const;
};

inline Board::Disk opponent(Board::Disk d) {
    return d == Board::Disk::X ? Board::Disk::O : Board::Disk::X;
}


