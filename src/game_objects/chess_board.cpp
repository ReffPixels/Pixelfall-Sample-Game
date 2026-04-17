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

// Draw every tile of the board and round the corners
void ChessBoard::drawRound(Painter& painter, float cornerRadius) {

    Vector4 radius{0.0f, 0.0f, 0.0f, 0.0f};

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            // Round Corners
            if      (file == 0 && rank == 0) radius = {cornerRadius, 0.0f, 0.0f, 0.0f};
            else if (file == 7 && rank == 0) radius = {0.0f, cornerRadius, 0.0f, 0.0f};
            else if (file == 7 && rank == 7) radius = {0.0f, 0.0f, cornerRadius, 0.0f};
            else if (file == 0 && rank == 7) radius = {0.0f, 0.0f, 0.0f, cornerRadius};
            
            painter.drawRectangleRound(
                {position.x + (file * tileSize.x), position.y + (rank * tileSize.y)},
                tileSize,
                ((file + rank + 1) % 2 == 0) ? darkTileColor : lightTileColor,
                radius
            );
            // Reset radius
            radius = {0.0f, 0.0f, 0.0f, 0.0f};
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
    return {getFileByDirection(static_cast<int>(squarePosition.x)),
        getRankByDirection(static_cast<int>(squarePosition.y))};
}

// Get file adjusted by the board direction (Flipped when black is at the bottom)
int ChessBoard::getFileByDirection(int file) {
    if (direction == BoardDirection::BlackOnTop)
        return file;
    else
        return 7 - file;
}

// Get rank adjusted by the board direction (Flipped when black is at the bottom)
int ChessBoard::getRankByDirection(int rank) {
    if (direction == BoardDirection::BlackOnTop)
        return rank;
    else
        return 7 - rank;
}