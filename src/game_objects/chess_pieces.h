// Defines a chess pieces object that handles drawing all the chess pieces on the screen.

#pragma once
// Engine
#include "pixelfall/engine/math/vector2.h"
#include "pixelfall/engine/graphics/painter.h"
// Standard Library
#include <array>
// Chess
#include "config/chess_config.h"
#include "game_objects/chess_board.h"

class ChessPieces {
public:
    // Rendering
    void drawPieces(std::vector<Piece>& pieces, ChessBoard& board, Painter& painter);
    void drawFree(PieceType type, TeamColor team, Vector2 position, Painter& painter, Vector2 size = Vector2::Zero);
    void pieceFollowCursor(Vector2& cursorPos, ChessBoard& board, PieceType type, TeamColor team,
        Painter& painter, Vector2 offset = Vector2::Zero);

    // Setters
    void setSpriteSize(Vector2 spriteSize) { this->spriteSize = spriteSize; };
    void setPieceOffset(Vector2 pieceOffset) { this->pieceOffset = pieceOffset; };
    void setSpritesPath(std::filesystem::path spritesPath) { this->spritesPath = spritesPath; };

private:
    // Visuals
    Vector2 spriteSize{70.0f, 70.0f};
    std::filesystem::path spritesPath{"assets/image/themes/Cburnett/"};
    Vector2 pieceOffset{Vector2::Zero};

    // Rendering Methods
    void drawPiece(Piece& piece, ChessBoard& board, Painter& painter);
    std::filesystem::path findImagePath(PieceType type, TeamColor team);
};