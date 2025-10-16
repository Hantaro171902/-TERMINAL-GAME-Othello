#pragma once

#include <string>
#include <vector>

#include "board.hpp"
#include "color.hpp"
#include "utils.hpp"

struct MoveRecord {
    int row;
    int col;
    Board::Disk player;
};

class Renderer {
public:
    void drawBoard(const Board & b, const std::vector<std::pair<int,int>> & valid, Board::Disk turn,
                   int cursorX, int cursorY) const;
    void drawSideMenu(const std::vector<MoveRecord>& history, int scoreX, int scoreO, 
                      Board::Disk currentTurn, int cursorX, int cursorY, int elapsedSeconds, int boardSize) const;
    void drawInstructions(int x, int y) const;
    void drawMoveHistory(const std::vector<MoveRecord>& history, int x, int y, int scrollOffset = 0) const;
};


