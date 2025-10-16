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
    clearTerminal();
    
    // Draw title
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

    // Draw board with box drawing characters
    int boardSize = b.getSize();
    int boardTop = 10;
    int boardLeft = 4;
    
    // Top border
    move_cursor(boardLeft, boardTop);
    setTextColor(TextColor::WHITE);
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
                cout << "[";
                if (d == Board::Disk::X) {
                    setTextColor(TextColor::RED);
                    cout << BLACK_CIRCLE;
                    resetTextColor();
                }
                else if (d == Board::Disk::O) {
                    setTextColor(TextColor::GREEN);
                    cout << WHITE_CIRCLE;
                    resetTextColor();
                } 
                else {
                    cout << ' ';
                }
                cout << "]";
            } else {
                if (d == Board::Disk::X) {
                    cout << " " << BLACK_CIRCLE << " ";
                }
                else if (d == Board::Disk::O) {
                    cout << " " << WHITE_CIRCLE << " ";
                }
                else if (isValidMove) {
                    // draw possible move dot in blue
                    setTextColor(TextColor::BRIGHT_BLUE);
                    cout << " . ";
                    resetTextColor();
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
    move_cursor(boardLeft, boardTop + boardSize * 2);
    cout << SYMBOL_DOUBLE_BOTTOM_LEFT;
    for (int i = 0; i < boardSize; i++) {
        cout << SYMBOL_DOUBLE_HORIZONTAL << SYMBOL_DOUBLE_HORIZONTAL << SYMBOL_DOUBLE_HORIZONTAL;
        if (i < boardSize - 1) cout << SYMBOL_DOUBLE_T_BOTTOM;
    }
    cout << SYMBOL_DOUBLE_BOTTOM_RIGHT;

    // Ensure output is flushed so the terminal receives the full board immediately
    cout << flush;

    // Draw coordinates
    setTextColor(TextColor::YELLOW);
    // Draw column coordinates (handle 1..9 and 10)
    for (int i = 0; i < boardSize; i++) {
        move_cursor(boardLeft + 2 + i * 4, boardTop - 1);
        if (i + 1 < 10)
            cout << (i + 1);
        else
            cout << (i + 1); // two-digit will occupy more space but move_cursor positions are approximate
    }
    // Draw row coordinates
    for (int i = 0; i < boardSize; i++) {
        move_cursor(boardLeft - 3, boardTop + 1 + i * 2);
        if (i + 1 < 10)
            cout << (i + 1);
        else
            cout << (i + 1);
    }
    resetTextColor();
}

void Renderer::drawSideMenu(const vector<MoveRecord>& history, int scoreX, int scoreO, 
                           Board::Disk currentTurn, int cursorX, int cursorY, int elapsedSeconds, int boardSize) const
{
    int menuY = 10;

    // Calculate board width in characters: each cell is 3 chars plus vertical lines
    int boardCharWidth = boardSize * 4 + 1; // approximate

    // Try to place menu to the right of the board; if terminal is small or board is large,
    // push menu further right or to the far right edge
    int termCols = 0, termRows = 0;
    if (!get_terminal_size(termCols, termRows)) termCols = 120; // fallback

    int boardLeft = 4;
    int proposedMenuX = boardLeft + boardCharWidth + 4;
    int menuX = proposedMenuX;
    if (menuX + 40 > termCols) {
        // shift menu to fit within terminal
        menuX = max(boardLeft + boardCharWidth + 2, termCols - 48);
        if (menuX < boardLeft + boardCharWidth + 1) menuX = boardLeft + boardCharWidth + 1;
    }
    
    // Draw score and timer
    move_cursor(menuX, menuY);
    setTextColor(TextColor::WHITE);
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
    setTextColor(TextColor::WHITE);
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
    setTextColor(TextColor::WHITE);
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
        cout << "║                                     ║";
    }
    
    move_cursor(x, y + 2 + maxMoves);
    cout << "╚─────────────────────────────────────╝";
    resetTextColor();
}


