// Defines a chess piece object and handles drawing it on the screen.

#pragma once
// Engine
#include "pixelfall/engine/math/vector2.h"
#include "pixelfall/engine/graphics/painter.h"

enum PieceType {
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn,
    None
};

enum PieceTeam {
    White,
    Black
};

struct PieceInfo {
    PieceType type;
    PieceTeam team;
    Vector2Int position{0, 0};
};

class ChessPieces {
public:
    // Methods
    void draw(PieceInfo pieceInfo, Vector2 boardPosition, Vector2 tileSize, Vector2 spriteSize,
        Painter& painter, Vector2 pieceOffset = Vector2::Zero);

    void drawPieces(std::vector<PieceInfo> pieces, Vector2 boardPosition, Vector2 tileSize, Vector2 spriteSize,
        Painter& painter, Vector2 pieceOffset = Vector2::Zero);

    void drawFree(PieceType type, PieceTeam team, Vector2 physicalPosition, Vector2 spriteSize,
        Painter& painter);

    // Setters
    void setIsPieceSelected(bool isPieceSelected) { this->isPieceSelected = isPieceSelected; };
    void setSelectedPieceIndex(int selectedPieceIndex) { this->selectedPieceIndex = selectedPieceIndex; };

    // Getters
    static Vector2Int getPosFromNotation(std::string gridPosition);
    static std::string getNotationFromPos(Vector2Int gridPosition);

private:
    bool isPieceSelected{false};
    int selectedPieceIndex{-1};
};