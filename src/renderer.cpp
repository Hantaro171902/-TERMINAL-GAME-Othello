#include "headers/renderer.hpp"

#include <iostream>
#include <set>

using namespace std;

static bool contains(const vector<pair<int,int>> & v, int x, int y)
{
    for (auto &p : v)
        if (p.first == x && p.second == y) return true;
    return false;
}

void Renderer::drawBoard(const Board & b, const vector<pair<int,int>> & valid, Board::Disk turn,
                         int cursorX, int cursorY) const
{
    clearScreen();

    cout << "  | 1 2 3 4 5 6 7 8 |\n";
    cout << "+-" << string(17, '-') << "+\n";

    for (int y = 0; y < 8; y++) {
        cout << (y + 1) << " |";
        for (int x = 0; x < 8; x++) {
            bool isCursor = x == cursorX && y == cursorY;
            if (isCursor) setTextColor(TextColor::BRIGHT_WHITE);

            Board::Disk d = b.get(x, y);
            if (d == Board::Disk::X) {
                setTextColor(TextColor::GREEN);
                cout << " X";
            }
            else if (d == Board::Disk::O) {
                setTextColor(TextColor::RED);
                cout << " O";
            }
            else if (contains(valid, x, y)) {
                setTextColor(TextColor::CYAN);
                cout << " .";
            }
            else {
                resetTextColor();
                cout << "  ";
            }

            resetTextColor();
        }
        cout << " |\n";
    }

    cout << "+-" << string(17, '-') << "+\n";

    int xs = b.count(Board::Disk::X);
    int os = b.count(Board::Disk::O);
    cout << "X's: " << xs << "\n";
    cout << "O's: " << os << "\n";
    cout << "Current turn: " << (turn == Board::Disk::X ? "X" : "O") << "\n";

    cout << "Valid moves: ";
    bool first = true;
    for (auto &p : valid) {
        if (!first) cout << ", ";
        cout << (p.first + 1) << " " << (p.second + 1);
        first = false;
    }
    cout << "\n";
}


