// Implementation for chess_board.h

#include "game_objects/chess_board.h"
#include "tools/notation_parser.h"

// Draw every tile of the board
void ChessBoard::draw(Painter& painter) {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
                painter.drawRectangle(
                    {position.x + (file * tileSize.x), position.y + (rank * tileSize.y)},
                    tileSize,
                    ((file + rank + 1) % 2 == 0) ? darkTileColor : lightTileColor
                );
        }
    }
}

// Check if the cursor is on top of the board
bool ChessBoard::isBoardOnHover(Vector2 mousePosition) {
    return (mousePosition.x > position.x
        && mousePosition.y > position.y
        && mousePosition.x < position.x + (tileSize.x * 8)
        && mousePosition.y < position.y + (tileSize.y * 8)
    );
}

// Get the specific tile under the cursor
Vector2Int ChessBoard::getTileOnHover(Vector2 mousePosition) {
    Vector2 squarePosition{(mousePosition - position) / tileSize.x};
    return {static_cast<int>(squarePosition.x),
        getRankByDirection(static_cast<int>(squarePosition.y))};
}

// Get rank adjusted by the board direction (Flipped when black is at the bottom)
int ChessBoard::getRankByDirection(int rank) {
    if (direction == BoardDirection::BlackOnTop)
        return rank;
    else
        return 7 - rank;
}