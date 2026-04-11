// UI

#pragma once
#include "game_objects/chess_pieces.h"

class PromotionInterface {
public:
    // Methods
    void drawPieces(ChessPieces& chessPieces, Vector2Int pawnPosition, TeamColor playerTeam,
        Vector2 boardPosition, Vector2 tileSize, Vector2 spriteSize,
        Painter& painter, bool flipPieces, Color backgroundColor = Color::fromHexcode("#fff"));
    
    PieceType getPieceOnHover(Vector2 mousePos, Vector2Int pawnPosition, TeamColor playerTeam,
        Vector2 boardPosition, Vector2 tileSize, bool flipPieces);
};