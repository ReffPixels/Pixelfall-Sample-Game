#include "game_objects/promotion_ui.h"

// Draws promotion piece options above/below the board
void PromotionInterface::drawPieces(ChessPieces& chessPieces, Vector2Int pawnPosition, PieceTeam playerTeam,
    Vector2 boardPosition, Vector2 tileSize, Vector2 spriteSize,
    Painter& painter, bool flipPieces, Color backgroundColor) {
        
    Vector2 physicalPawnPosition{
        boardPosition.x
        + (float)pawnPosition.x
        * tileSize.x
        ,
        boardPosition.y
        + (flipPieces ? 7 - (float)pawnPosition.y : (float)pawnPosition.y)
        * tileSize.y
    };

    Vector2 queenPosition{ physicalPawnPosition.x - tileSize.x * 1.5f,
        physicalPawnPosition.y + (flipPieces ? -tileSize.y : tileSize.y)};
    Vector2 rookPosition{physicalPawnPosition.x - tileSize.x * 0.5f,
        physicalPawnPosition.y + (flipPieces ? -tileSize.y : tileSize.y)};
    Vector2 bishopPosition{physicalPawnPosition.x + tileSize.x * 0.5f,
        physicalPawnPosition.y + (flipPieces ? -tileSize.y : tileSize.y)};
    Vector2 knightPosition{physicalPawnPosition.x + tileSize.x * 1.5f,
        physicalPawnPosition.y + (flipPieces ? -tileSize.y : tileSize.y)};

    chessPieces.drawFree(PieceType::Queen, playerTeam, queenPosition, spriteSize, painter);
    chessPieces.drawFree(PieceType::Rook, playerTeam, rookPosition, spriteSize, painter);
    chessPieces.drawFree(PieceType::Bishop, playerTeam, bishopPosition, spriteSize, painter);
    chessPieces.drawFree(PieceType::Knight, playerTeam, physicalPawnPosition, spriteSize, painter);
}

// Get the specific square under the cursor
std::string PromotionInterface::getSquareOnHover(Vector2 mousePosition) {
        // Vector2 squarePosition{(mousePosition - boardPosition) / tileSize.x};
        // return ChessPieces::getNotationFromPos({(int)squarePosition.x, getRankByDirection((int)squarePosition.y)});
        return "woah";
}