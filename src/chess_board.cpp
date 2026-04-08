// Implementation for chess_board.h
#include "chess_board.h"
#include "config/board_config.h"

void ChessBoard::draw(Painter& painter) {
    
    Vector2 adjustedPosition{
        boardPosition.x - ((tileSize.x * 8) / 2.0f),
        boardPosition.y - ((tileSize.y * 8) / 2.0f)
    };

    // Draw board
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if (projectionType == ThemeProjection::Isometric) {
                // Center of this tile in screen space
                float cx = adjustedPosition.x + (file - rank) * (tileSize.x / 2);
                float cy = adjustedPosition.y + (file + rank) * (tileSize.y / 2);

                // Four corners of the diamond
                Vector2 top{cx,         cy - (tileSize.y / 2)};
                Vector2 right{cx + (tileSize.x / 2), cy};
                Vector2 bot{cx,         cy + (tileSize.y / 2)};
                Vector2 left{cx - (tileSize.x / 2), cy};

                painter.drawQuad(
                    top, right, bot, left,
                    ((file + rank + 1) % 2 == 0) ? darkSquareColor : lightSquareColor
                );
            }
            else {
                painter.drawRectangle(
                    {adjustedPosition.x + (file * tileSize.x), adjustedPosition.y + (rank * tileSize.y)},
                    tileSize,
                    ((file + rank + 1) % 2 == 0) ? darkSquareColor : lightSquareColor
                );
            }
        }
    }
}