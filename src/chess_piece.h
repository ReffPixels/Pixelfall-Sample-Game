// Defines a chess piece object

#pragma once
#include "pixelfall/engine/math/vector2.h"
#include "pixelfall/engine/graphics/painter.h"
#include "pixelfall/engine/graphics/texture.h"
#include "pixelfall/engine/math/vector2.h"
#include <optional>

enum PieceType {
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn
};

enum PieceTeam {
    White,
    Black
};

class ChessPiece {
public:
    // Constructor
    ChessPiece(std::string gridPosition, PieceType pieceType, PieceTeam pieceTeam, const std::filesystem::path& projectPath)
        : gridPosition(gridPosition), pieceType(pieceType), pieceTeam(pieceTeam) {
    }

    // Getters
    const std::string getPosition() const { return gridPosition; };
    const PieceType getPieceType() const { return pieceType; };

    // Setters
    void setGridPosition(const std::string gridPosition) { this->gridPosition = gridPosition; };
    void setPieceType(const PieceType pieceType) { this->pieceType = pieceType; };

    // Methods
    void draw(Painter& painter, const std::filesystem::path& projectPath, Vector2 boardPosition, Vector2 squareSize);
    std::filesystem::path findImagePath(const std::filesystem::path& projectPath);

    // Destructor
    ~ChessPiece() = default;
private:
    // Position is stored in chess notation (a1 to h8)
    std::string gridPosition;
    PieceType pieceType;
    PieceTeam pieceTeam;
    std::optional<Texture> texture;
};