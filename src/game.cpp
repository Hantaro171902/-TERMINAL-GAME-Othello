#include "headers/game.hpp"

#include <iostream>

using namespace std;

Game::Game() : board(true), turn(Board::Disk::X), cursorX(0), cursorY(0)
{
}

void Game::run()
{
    hideCursor();
    bool quit = false;

    while (!quit) {
        auto valid = board.getValid(turn);
        renderer.drawBoard(board, valid, turn, cursorX, cursorY);

        InputKey key = getInputKey();
        switch (key) {
            case InputKey::LEFT:  if (cursorX > 0) cursorX--; break;
            case InputKey::RIGHT: if (cursorX < 7) cursorX++; break;
            case InputKey::UP:    if (cursorY > 0) cursorY--; break;
            case InputKey::DOWN:  if (cursorY < 7) cursorY++; break;
            case InputKey::ENTER:
            {
                if (board.isValid(cursorX, cursorY, turn)) {
                    board.put(cursorX, cursorY, turn);
                    turn = opponent(turn);
                }
                break;
            }
            case InputKey::R:
                board.reset();
                turn = Board::Disk::X;
                break;
            case InputKey::ESC:
            case InputKey::Q:
                quit = true;
                break;
            default:
                break;
        }

        // if current player has no moves, pass turn
        if (board.getValid(turn).empty()) {
            turn = opponent(turn);
            if (board.getValid(turn).empty()) {
                renderer.drawBoard(board, {}, turn, cursorX, cursorY);
                cout << "Game over. Press Q to quit." << endl;
                while (true) {
                    InputKey k = getInputKey();
                    if (k == InputKey::Q || k == InputKey::ESC || k == InputKey::ENTER) {
                        quit = true;
                        break;
                    }
                }
            }
        }
    }

    showCursor();
}


