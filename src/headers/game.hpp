#pragma once

#include <vector>

#include "board.hpp"
#include "renderer.hpp"
#include "cursor_input.hpp"

class Game {
    Board board;
    Renderer renderer;
    Board::Disk turn;
    int cursorX;
    int cursorY;

public:
    Game();
    void run();
};


