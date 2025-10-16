#pragma once

#include <vector>
#include <chrono>

#include "board.hpp"
#include "renderer.hpp"
#include "cursor_input.hpp"
#include "utils.hpp"

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
    // Show winner message. If posX/posY >= 0 they specify the column,row to print the message.
    void isWinner(int posX = -1, int posY = -1);
};


