// Implementation for game_objects/chess_board.h
#include "game_objects/chess_board.h"
#include "config/board_config.h"
#include "game_objects/chess_pieces.h"

// Draw board (Depending on projection type)
void ChessBoard::draw(Painter& painter) {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
                painter.drawRectangle(
                    {boardPosition.x + (file * tileSize.x), boardPosition.y + (rank * tileSize.y)},
                    tileSize,
                    ((file + rank + 1) % 2 == 0) ? darkSquareColor : lightSquareColor
                );
        }
    }
}

// Check if the cursor is on top of the board
bool ChessBoard::isBoardOnHover(Vector2 mousePosition) {
    return (mousePosition.x > boardPosition.x
        && mousePosition.y > boardPosition.y
        && mousePosition.x < boardPosition.x + (tileSize.x * 8)
        && mousePosition.y < boardPosition.y + (tileSize.y * 8)
    );
}

// Get the specific square under the cursor
std::string ChessBoard::getSquareOnHover(Vector2 mousePosition) {
    Vector2 squarePosition{(mousePosition - boardPosition) / tileSize.x};
    return ChessPieces::getNotationFromPos({(int)squarePosition.x, getRankByDirection((int)squarePosition.y)});
}

// Get rank adjusted by the board direction (Flipped when black is at the bottom)
int ChessBoard::getRankByDirection(int rank) {
    if (boardDirection == BoardDirection::BlackOnTop)
        return rank;
    else
        return 7 - rank;
}