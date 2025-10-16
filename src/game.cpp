#include "game.hpp"

#include <iostream>
#include <thread>

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
    
    // Use a non-blocking loop so timer updates even when no keys are pressed
    while (isRunning) {
        auto valid = board->getValid(turn);
        renderer.drawBoard(*board, valid, turn, cursorX, cursorY);
        renderer.drawSideMenu(moveHistory, board->count(Board::Disk::X), board->count(Board::Disk::O), 
                 turn, cursorX, cursorY, getElapsedSeconds(), boardSize);

        // Poll input non-blocking
        InputKey key = InputKey::NONE;
        if (kbhit()) {
            int ch = getch();
            // put character back for existing getInputKey logic that expects to read
            // but since getInputKey handles termios itself, we map common keys here
            if (ch == '\n') key = InputKey::ENTER;
            else if (ch == 27) {
                // possible arrow escape sequence
                if (kbhit()) {
                    int c1 = getch();
                    if (c1 == '[' && kbhit()) {
                        int c2 = getch();
                        if (c2 == 'A') key = InputKey::UP;
                        else if (c2 == 'B') key = InputKey::DOWN;
                        else if (c2 == 'C') key = InputKey::RIGHT;
                        else if (c2 == 'D') key = InputKey::LEFT;
                    }
                } else {
                    key = InputKey::ESC;
                }
            }
            else if (ch == 'w' || ch == 'W') key = InputKey::UP;
            else if (ch == 's' || ch == 'S') key = InputKey::DOWN;
            else if (ch == 'a' || ch == 'A') key = InputKey::LEFT;
            else if (ch == 'd' || ch == 'D') key = InputKey::RIGHT;
            else if (ch == 'q' || ch == 'Q') key = InputKey::Q;
            else if (ch == 'r' || ch == 'R') key = InputKey::R;
            else if (ch == '[') key = InputKey::LEFT_BRACKET;
            else if (ch == ']') key = InputKey::RIGHT_BRACKET;
            else if (ch >= '1' && ch <= '9') {
                // allow quick number input to change board size mid-game (optional)
            }
        } else {
            // No key pressed: small sleep so CPU not pegged, timer will still update on next loop
            sleep_ms(50);
        }

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
                                     turn, cursorX, cursorY, getElapsedSeconds(), boardSize);
                
                // Show game over message
                // Display winner/tie message
                isWinner(20,20);

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
    cout << endl << endl;
    cout << " ██████╗ ████████╗██╗  ██╗███████╗██╗     ██╗      ██████╗ " << endl;
    cout << "██╔═══██╗╚══██╔══╝██║  ██║██╔════╝██║     ██║     ██╔═══██╗" << endl;
    cout << "██║   ██║   ██║   ███████║█████╗  ██║     ██║     ██║   ██║" << endl;
    cout << "██║   ██║   ██║   ██╔══██║██╔══╝  ██║     ██║     ██║   ██║" << endl;
    cout << "╚██████╔╝   ██║   ██║  ██║███████╗███████╗███████╗╚██████╔╝" << endl;
    cout << " ╚═════╝    ╚═╝   ╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝ ╚═════╝ " << endl;        
    resetTextColor();

    cout << endl << endl;
    
    move_cursor(20, 10);
    setTextColor(TextColor::WHITE);
    cout << "╔══════════════════════════════════════╗";
    move_cursor(20, 11);
    cout << "║           SELECT BOARD SIZE          ║";
    move_cursor(20, 12);
    cout << "╠══════════════════════════════════════╣";
    move_cursor(20, 13);
    cout << "║  1. 8x8  (Classic)                   ║";
    move_cursor(20, 14);
    cout << "║  2. 10x10 (Medium)                   ║";
    move_cursor(20, 15);
    cout << "║  3. 12x12 (Large)                    ║";
    move_cursor(20, 16); 
    cout << "║  Q. Quit                             ║";
    move_cursor(20, 17);
    cout << "╚══════════════════════════════════════╝";
    resetTextColor();
    // Ensure menu prints immediately
    cout << flush;

    while (true) {
        // Non-blocking check for number or quit keys
        if (kbhit()) {
            int ch = getch();
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
            } else if (ch == 'q' || ch == 'Q' || ch == 27) {
                board = nullptr;
                isRunning = false;
                return;
            }
        }
        // small sleep to avoid busy loop
        sleep_ms(50);
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

void Game::isWinner(int posX, int posY) {
    // Determine winner based on current board counts
    if (!board) return;
    int countX = board->count(Board::Disk::X);
    int countO = board->count(Board::Disk::O);

    string msg;
    if (countX > countO) msg = string("Player ") + BLACK_CIRCLE + " wins!";
    else if (countO > countX) msg = string("Player ") + WHITE_CIRCLE + " wins!";
    else msg = "It's a tie!";

    int cx = posX;
    int cy = posY;
    if (cx < 0 || cy < 0) {
        int cols = 80, rows = 24;
        if (get_terminal_size(cols, rows)) {
            cx = cols / 2 - (int)msg.size() / 2;
            cy = rows / 2;
        } else {
            cx = 10; cy = 10; // fallback
        }
    }

    move_cursor(max(1, cx), max(1, cy));
    setTextColor(TextColor::BRIGHT_GREEN);
    cout << msg;
    resetTextColor();
    cout << flush;
    // pause so the player sees the message
    this_thread::sleep_for(chrono::seconds(3));
}


