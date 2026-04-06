// Implementation for piece.h
#include "chess_piece.h"

static Vector2Int gridNotationToVector(std::string gridPosition) {
    return Vector2Int{
        gridPosition[0] - 'a', // 'a' -> 0, 'h' -> 7
        '8' - gridPosition[1]  // '8' -> 0, '1' -> 7
    };
}

void ChessPiece::draw(
    Painter& painter, const std::filesystem::path& projectPath, Vector2 boardPosition, Vector2 squareSize) {

    std::filesystem::path imagePath = findImagePath(projectPath);

    Vector2 physicalPosition = {
        boardPosition.x + ((float)gridNotationToVector(gridPosition).x * squareSize.x),
        boardPosition.y + ((float)gridNotationToVector(gridPosition).y * squareSize.y),
    };
    if (!texture.has_value()) {
        texture.emplace(findImagePath(projectPath));
    }

    painter.drawSprite(
        physicalPosition,
        squareSize,
        *texture
    );
}

std::filesystem::path ChessPiece::findImagePath(const std::filesystem::path& projectPath) {
    if (pieceTeam == PieceTeam::White) {
        switch (pieceType) {
        case King: return (projectPath / "assets/image/pieces/white_king.png");
        case Queen: return (projectPath / "assets/image/pieces/white_queen.png");
        case Rook: return (projectPath / "assets/image/pieces/white_rook.png");
        case Bishop: return (projectPath / "assets/image/pieces/white_bishop.png");
        case Knight: return (projectPath / "assets/image/pieces/white_knight.png");
        case Pawn: return (projectPath / "assets/image/pieces/white_pawn.png");
        default: return projectPath; // Error, invalid piece type
        }
    }
    else {
        switch (pieceType) {
        case King: return (projectPath / "assets/image/pieces/black_king.png");
        case Queen: return (projectPath / "assets/image/pieces/black_queen.png");
        case Rook: return (projectPath / "assets/image/pieces/black_rook.png");
        case Bishop: return (projectPath / "assets/image/pieces/black_bishop.png");
        case Knight: return (projectPath / "assets/image/pieces/black_knight.png");
        case Pawn: return (projectPath / "assets/image/pieces/black_pawn.png");
        default: return projectPath; // Error, invalid piece type
        }
    }
}