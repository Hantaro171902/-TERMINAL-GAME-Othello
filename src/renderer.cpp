#include "renderer.hpp"

#include <iostream>
#include <set>
#include <iomanip>

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
    
    // Draw title
    setTextColor(TextColor::YELLOW);
    cout << "                  ______     __  __     _____     ______     __  __     __  __    \n";
    cout << "                 /\\  ___\\   /\\ \\/\\ \\   /\\  __-.  /\\  __ \\   /\\ \\/ /    /\\ \\/\\ \\   \n";
    cout << "                 \\ \\___  \\  \\ \\ \\_\\ \\  \\ \\ \\/\\ \\ \\ \\ \\/\\ \\  \\ \\  _\"-.  \\ \\ \\_\\ \\  \n";
    cout << "                  \\/\\_____\\  \\ \\_____\\  \\ \\____-  \\ \\_____\\  \\ \\_\\ \\_\\  \\ \\_____\\ \n";
    cout << "                   \\/_____/   \\/_____/   \\/____/   \\/_____/   \\/_/\\/_/   \\/_____/ \n";
    resetTextColor();

    // Draw board with box drawing characters
    int boardSize = b.getSize();
    int boardTop = 8;
    int boardLeft = 4;
    
    // Top border
    move_cursor(boardLeft, boardTop);
    setTextColor(TextColor::YELLOW);
    cout << SYMBOL_DOUBLE_TOP_LEFT;
    for (int i = 0; i < boardSize; i++) {
        cout << SYMBOL_DOUBLE_HORIZONTAL << SYMBOL_DOUBLE_HORIZONTAL << SYMBOL_DOUBLE_HORIZONTAL;
        if (i < boardSize - 1) cout << SYMBOL_DOUBLE_T_TOP;
    }
    cout << SYMBOL_DOUBLE_TOP_RIGHT;

    // Board content
    for (int y = 0; y < boardSize; y++) {
        // Content row
        move_cursor(boardLeft, boardTop + 1 + y * 2);
        cout << SYMBOL_DOUBLE_VERTICAL;
        for (int x = 0; x < boardSize; x++) {
            bool isCursor = x == cursorX && y == cursorY;
            bool isValidMove = contains(valid, x, y);
            
            Board::Disk d = b.get(x, y);
            
            if (isCursor) {
                setTextColor(TextColor::BRIGHT_WHITE);
                cout << "[" << (d == Board::Disk::X ? BLACK_CIRCLE : d == Board::Disk::O ? WHITE_CIRCLE : " ") << "]";
            } else {
                if (d == Board::Disk::X) {
                    cout << " " << BLACK_CIRCLE << " ";
                }
                else if (d == Board::Disk::O) {
                    cout << " " << WHITE_CIRCLE << " ";
                }
                else if (isValidMove) {
                    cout << " . ";
                }
                else {
                    cout << "   ";
                }
            }
            resetTextColor();
            cout << SYMBOL_DOUBLE_VERTICAL;
        }

        // Separator row (except for last row)
        if (y < boardSize - 1) {
            move_cursor(boardLeft, boardTop + 2 + y * 2);
            cout << SYMBOL_DOUBLE_T_LEFT;
            for (int x = 0; x < boardSize; x++) {
                cout << SYMBOL_DOUBLE_HORIZONTAL << SYMBOL_DOUBLE_HORIZONTAL << SYMBOL_DOUBLE_HORIZONTAL;
                if (x < boardSize - 1) cout << SYMBOL_DOUBLE_INTERSECT;
            }
            cout << SYMBOL_DOUBLE_T_RIGHT;
        }
    }

    // Bottom border
    move_cursor(boardLeft, boardTop + 1 + boardSize * 2);
    cout << SYMBOL_DOUBLE_BOTTOM_LEFT;
    for (int i = 0; i < boardSize; i++) {
        cout << SYMBOL_DOUBLE_HORIZONTAL << SYMBOL_DOUBLE_HORIZONTAL << SYMBOL_DOUBLE_HORIZONTAL;
        if (i < boardSize - 1) cout << SYMBOL_DOUBLE_T_BOTTOM;
    }
    cout << SYMBOL_DOUBLE_BOTTOM_RIGHT;

    // Draw coordinates
    setTextColor(TextColor::YELLOW);
    for (int i = 0; i < boardSize; i++) {
        move_cursor(boardLeft + 2 + i * 4, boardTop - 1);
        cout << (char)('1' + i);
    }
    for (int i = 0; i < boardSize; i++) {
        move_cursor(boardLeft - 2, boardTop + 1 + i * 2);
        cout << (char)('1' + i);
    }
    resetTextColor();
}

void Renderer::drawSideMenu(const vector<MoveRecord>& history, int scoreX, int scoreO, 
                           Board::Disk currentTurn, int cursorX, int cursorY, int elapsedSeconds) const
{
    int menuX = 45;
    int menuY = 8;
    
    // Draw score and timer
    move_cursor(menuX, menuY);
    setTextColor(TextColor::YELLOW);
    cout << "╔═══════╦════╦═════╦═════╦════╦═══════╗";
    move_cursor(menuX, menuY + 1);
    cout << "║ " << formatTime(elapsedSeconds) << " ║ " << setw(2) << scoreX << " ║  " << BLACK_CIRCLE << "  ║  " << WHITE_CIRCLE << "  ║ " << setw(2) << scoreO << " ║ " << formatTime(elapsedSeconds) << " ║";
    move_cursor(menuX, menuY + 2);
    cout << "╚═══════╩════╩═════╩═════╩════╩═══════╝";
    
    // Draw current turn indicator
    move_cursor(menuX, menuY + 4);
    setTextColor(TextColor::BRIGHT_WHITE);
    cout << "Current turn: ";
    if (currentTurn == Board::Disk::X) {
        cout << BLACK_CIRCLE;
    } else {
        cout << WHITE_CIRCLE;
    }
    
    // Draw cursor position
    move_cursor(menuX, menuY + 5);
    resetTextColor();
    cout << "Cursor: " << (char)('A' + cursorX) << (cursorY + 1);
    
    // Draw move history
    drawMoveHistory(history, menuX, menuY + 7, 0);
    
    // Draw instructions
    drawInstructions(menuX, menuY + 20);
}

void Renderer::drawInstructions(int x, int y) const
{
    move_cursor(x, y);
    setTextColor(TextColor::YELLOW);
    cout << "╔═════════════════════════════════════╗";
    move_cursor(x, y + 1);
    cout << "║ ↑←↓→  : Move cursor    ENTER : Place║";
    move_cursor(x, y + 2);
    cout << "║ WASD  : Move cursor    R     : Reset║";
    move_cursor(x, y + 3);
    cout << "║ Q     : Quit game      ESC   : Quit ║";
    move_cursor(x, y + 4);
    cout << "╚═════════════════════════════════════╝";
    resetTextColor();
}

void Renderer::drawMoveHistory(const vector<MoveRecord>& history, int x, int y, int scrollOffset) const
{
    move_cursor(x, y);
    setTextColor(TextColor::YELLOW);
    cout << "              Historical moves";
    move_cursor(x, y + 1);
    cout << "╔─────────────────────────────────────╗";
    
    int maxMoves = 10;
    int startIdx = max(0, (int)history.size() - maxMoves - scrollOffset);
    int endIdx = min((int)history.size(), startIdx + maxMoves);
    
    for (int i = startIdx; i < endIdx; i++) {
        move_cursor(x, y + 2 + (i - startIdx));
        cout << "║ " << setw(2) << (i + 1) << ". ";
        
        if (history[i].player == Board::Disk::X) {
            cout << BLACK_CIRCLE;
        } else {
            cout << WHITE_CIRCLE;
        }
        resetTextColor();
        cout << " (" << (char)('A' + history[i].col) << (history[i].row + 1) << ")";
        
        // Fill remaining space
        int remaining = 35 - (4 + 1 + 4); // 4 for number, 1 for player, 4 for position
        for (int j = 0; j < remaining; j++) cout << " ";
        cout << "║";
    }
    
    // Fill empty lines if needed
    for (int i = endIdx - startIdx; i < maxMoves; i++) {
        move_cursor(x, y + 2 + i);
        cout << "║                                   ║";
    }
    
    move_cursor(x, y + 2 + maxMoves);
    cout << "╚─────────────────────────────────────╝";
    resetTextColor();
}


