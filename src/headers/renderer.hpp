#pragma once

#include <string>
#include <vector>

#include "board.hpp"
#include "color.hpp"
#include "utils.hpp"

class Renderer {
public:
    void drawBoard(const Board & b, const std::vector<std::pair<int,int>> & valid, Board::Disk turn,
                   int cursorX, int cursorY) const;
};


