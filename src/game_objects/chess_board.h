// Defines a chess piece object and handles drawing it on the screen.

#pragma once
// Engine
#include "pixelfall/engine/math/vector2.h"
#include "pixelfall/engine/graphics/painter.h"
#include "config/chess_theme.h"

enum class BoardDirection {
    BlackOnTop,
    WhiteOnTop
};

class ChessBoard {
public:
    // Constructor
    ChessBoard(Vector2 boardPosition = Vector2::Zero) : boardPosition(boardPosition) {}

    // Getters
    const Vector2& getPosition() const { return boardPosition; };
    const Vector2& getTileSize() const { return tileSize; };
    const BoardDirection& getBoardDirection() const { return boardDirection; };
    int getRankByDirection(int rank);

    // Setters
    void setPosition(Vector2 boardPosition) { this->boardPosition = boardPosition; };
    void setBoardDirection(BoardDirection boardDirection) { this->boardDirection = boardDirection; };

    // Methods
    void draw(Painter& painter);
    bool isBoardOnHover(Vector2 mousePosition);
    std::string getSquareOnHover(Vector2 mousePosition);
private:
    // Board Settings
    Vector2 boardPosition{Vector2::Zero};
    BoardDirection boardDirection{BoardDirection::BlackOnTop};

    // Theme
    Vector2 tileSize{80.0f, 80.0f};  // width:height = 2:1
    Color lightSquareColor{Color::fromHexcode("#edd6b0")};
    Color darkSquareColor{Color::fromHexcode("#b88762")};
    ThemeProjection projectionType = ThemeProjection::Rectangular;
};