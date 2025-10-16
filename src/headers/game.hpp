#pragma once

#include <vector>
#include <chrono>

#include "board.hpp"
#include "renderer.hpp"
#include "cursor_input.hpp"

class Game {
    Board* board;
    Renderer renderer;
    Board::Disk turn;
    int cursorX;
    int cursorY;
    std::vector<MoveRecord> moveHistory;
    std::chrono::steady_clock::time_point startTime;
    bool isRunning;
    int boardSize;

public:
    Game();
    ~Game();
    void run();
    void showMenu();
    void resetGame();
    int getElapsedSeconds() const;
};


