#include "game.hpp"

#include <iostream>

using namespace std;

Game::Game() : board(nullptr), turn(Board::Disk::X), cursorX(0), cursorY(0), isRunning(true), boardSize(8)
{
    startTime = std::chrono::steady_clock::now();
}

Game::~Game()
{
    delete board;
}

void Game::run()
{
    showMenu();
    
    if (!board) return; // User cancelled menu
    
    hideCursor();
    
    while (isRunning) {
        auto valid = board->getValid(turn);
        renderer.drawBoard(*board, valid, turn, cursorX, cursorY);
        renderer.drawSideMenu(moveHistory, board->count(Board::Disk::X), board->count(Board::Disk::O), 
                             turn, cursorX, cursorY, getElapsedSeconds());

        InputKey key = getInputKey();
        switch (key) {
            case InputKey::LEFT:  
                if (cursorX > 0) cursorX--; 
                break;
            case InputKey::RIGHT: 
                if (cursorX < boardSize - 1) cursorX++; 
                break;
            case InputKey::UP:    
                if (cursorY > 0) cursorY--; 
                break;
            case InputKey::DOWN:  
                if (cursorY < boardSize - 1) cursorY++; 
                break;
            case InputKey::ENTER:
            {
                if (board->isValid(cursorX, cursorY, turn)) {
                    board->put(cursorX, cursorY, turn);
                    
                    // Record the move
                    MoveRecord move;
                    move.row = cursorY;
                    move.col = cursorX;
                    move.player = turn;
                    moveHistory.push_back(move);
                    
                    turn = opponent(turn);
                }
                break;
            }
            case InputKey::R:
                resetGame();
                break;
            case InputKey::ESC:
            case InputKey::Q:
                isRunning = false;
                break;
            default:
                break;
        }

        // if current player has no moves, pass turn
        if (board->getValid(turn).empty()) {
            turn = opponent(turn);
            if (board->getValid(turn).empty()) {
                // Game over
                renderer.drawBoard(*board, {}, turn, cursorX, cursorY);
                renderer.drawSideMenu(moveHistory, board->count(Board::Disk::X), board->count(Board::Disk::O), 
                                     turn, cursorX, cursorY, getElapsedSeconds());
                
                // Show game over message
                move_cursor(4, 25);
                setTextColor(TextColor::BRIGHT_WHITE);
                cout << "Game over! Press Q to quit or R to restart.";
                resetTextColor();
                
                while (true) {
                    InputKey k = getInputKey();
                    if (k == InputKey::Q || k == InputKey::ESC) {
                        isRunning = false;
                        break;
                    } else if (k == InputKey::R) {
                        resetGame();
                        break;
                    }
                }
            }
        }
    }

    showCursor();
}

void Game::showMenu()
{
    clearScreen();
    
    setTextColor(TextColor::YELLOW);
    cout << "                  ______     __  __     _____     ______     __  __     __  __    \n";
    cout << "                 /\\  ___\\   /\\ \\/\\ \\   /\\  __-.  /\\  __ \\   /\\ \\/ /    /\\ \\/\\ \\   \n";
    cout << "                 \\ \\___  \\  \\ \\ \\_\\ \\  \\ \\ \\/\\ \\ \\ \\ \\/\\ \\  \\ \\  _\"-.  \\ \\ \\_\\ \\  \n";
    cout << "                  \\/\\_____\\  \\ \\_____\\  \\ \\____-  \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_____\\ \n";
    cout << "                   \\/_____/   \\/_____/   \\/____/   \\/_____/   \\/_/\\/_/   \\/_____/ \n";
    resetTextColor();
    
    move_cursor(20, 10);
    setTextColor(TextColor::YELLOW);
    cout << "╔══════════════════════════════════════╗";
    move_cursor(20, 11);
    cout << "║           SELECT BOARD SIZE          ║";
    move_cursor(20, 12);
    cout << "╠══════════════════════════════════════╣";
    move_cursor(20, 13);
    cout << "║  1. 8x8  (Classic)                  ║";
    move_cursor(20, 14);
    cout << "║  2. 10x10 (Medium)                  ║";
    move_cursor(20, 15);
    cout << "║  3. 12x12 (Large)                   ║";
    move_cursor(20, 16);
    cout << "║  Q. Quit                            ║";
    move_cursor(20, 17);
    cout << "╚══════════════════════════════════════╝";
    resetTextColor();
    
    while (true) {
        InputKey key = getInputKey();
        switch (key) {
            case InputKey::ENTER:
                // This will be handled by the number keys
                break;
            case InputKey::Q:
            case InputKey::ESC:
                board = nullptr;
                isRunning = false;
                return;
            default:
                // Check for number keys
                if (key == InputKey::NONE) {
                    // Try to get the actual character
                    char ch = getch();
                    if (ch == '1') {
                        boardSize = 8;
                        board = new Board(8, true);
                        return;
                    } else if (ch == '2') {
                        boardSize = 10;
                        board = new Board(10, true);
                        return;
                    } else if (ch == '3') {
                        boardSize = 12;
                        board = new Board(12, true);
                        return;
                    } else if (ch == 'q' || ch == 'Q') {
                        board = nullptr;
                        isRunning = false;
                        return;
                    }
                }
                break;
        }
    }
}

void Game::resetGame()
{
    if (board) {
        board->reset();
    }
    turn = Board::Disk::X;
    cursorX = 0;
    cursorY = 0;
    moveHistory.clear();
    startTime = std::chrono::steady_clock::now();
}

int Game::getElapsedSeconds() const
{
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime);
    return (int)elapsed.count();
}


