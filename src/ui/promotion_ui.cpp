//  [TODO] Clean up

#include "ui/promotion_ui.h"

// Draws promotion piece options above/below the board
void PromotionInterface::drawPieces(ChessPieces& chessPieces, Vector2Int pawnPosition, TeamColor playerTeam,
    Vector2 boardPosition, Vector2 tileSize, Vector2 spriteSize,
    Painter& painter, bool flipPieces, Color backgroundColor) {
        
    Vector2 physicalPawnPosition{boardPosition.x + (float)pawnPosition.x * tileSize.x,
        boardPosition.y + (flipPieces ? 7 - (float)pawnPosition.y : (float)pawnPosition.y) * tileSize.y};

    bool flipUIPosition{(playerTeam == TeamColor::White && flipPieces) || (playerTeam == TeamColor::Black && !flipPieces)};

    Vector2 backgroundPosition{physicalPawnPosition.x - tileSize.x * 1.5f,
        physicalPawnPosition.y + (flipUIPosition ? tileSize.y : -tileSize.y)};

    Vector2 spriteOffset{(tileSize.x - spriteSize.x) / 2, (tileSize.y - spriteSize.y) / 2};

    Vector2 queenPosition{physicalPawnPosition.x + spriteOffset.x - tileSize.x * 1.5f,
        physicalPawnPosition.y + spriteOffset.y + (flipUIPosition ? tileSize.y : -tileSize.y)};
    
    Vector2 rookPosition{physicalPawnPosition.x + spriteOffset.x - tileSize.x * 0.5f,
        physicalPawnPosition.y + spriteOffset.y + (flipUIPosition ? tileSize.y : -tileSize.y)};
    
    Vector2 bishopPosition{physicalPawnPosition.x + spriteOffset.x + tileSize.x * 0.5f,
        physicalPawnPosition.y + spriteOffset.y + (flipUIPosition ? tileSize.y : -tileSize.y)};
    
    Vector2 knightPosition{physicalPawnPosition.x + spriteOffset.x + tileSize.x * 1.5f,
        physicalPawnPosition.y + spriteOffset.y + (flipUIPosition ? tileSize.y : -tileSize.y)};

    painter.drawRectangle(backgroundPosition, Vector2{tileSize.x * 4, tileSize.y}, backgroundColor);
    chessPieces.drawFree(PieceType::Queen, playerTeam, queenPosition, painter, spriteSize);
    chessPieces.drawFree(PieceType::Rook, playerTeam, rookPosition, painter, spriteSize);
    chessPieces.drawFree(PieceType::Bishop, playerTeam, bishopPosition, painter, spriteSize);
    chessPieces.drawFree(PieceType::Knight, playerTeam, knightPosition, painter, spriteSize);
}

// Get the specific square under the cursor
PieceType PromotionInterface::getPieceOnHover(Vector2 mousePos, Vector2Int pawnPosition, TeamColor playerTeam,
    Vector2 boardPosition, Vector2 tileSize, bool flipPieces) {

    Vector2 physicalPawnPosition{boardPosition.x + (float)pawnPosition.x * tileSize.x,
        boardPosition.y + (flipPieces ? 7 - (float)pawnPosition.y : (float)pawnPosition.y) * tileSize.y};

    bool flipUIPosition{(playerTeam == TeamColor::White && flipPieces) || (playerTeam == TeamColor::Black && !flipPieces)};
    
    Vector2 backgroundPosition{physicalPawnPosition.x - tileSize.x * 1.5f,
    physicalPawnPosition.y + (flipUIPosition ? tileSize.y : -tileSize.y)};

    // Check that the mouse is inside the UI
    if (mousePos.x < backgroundPosition.x || mousePos.x > backgroundPosition.x + tileSize.x * 4) return PieceType::None;
    if (mousePos.y < backgroundPosition.y || mousePos.y > backgroundPosition.y + tileSize.y) return PieceType::None;

    switch ((int)((mousePos.x - backgroundPosition.x) / tileSize.x)) {
    case 0: return PieceType::Queen;
    case 1: return PieceType::Rook;
    case 2: return PieceType::Bishop;
    case 3: return PieceType::Knight;
    default: return PieceType::None;
    }
}