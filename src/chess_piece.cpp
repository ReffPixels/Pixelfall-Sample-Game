// Implementation for piece.h
#include "chess_piece.h"

// Converts a piece location from traditional notation (a1 to h8) into a vector. [FUTURE-PROOFING]
static Vector2Int gridNotationToVector(std::string gridPosition) {
    return Vector2Int{
        gridPosition[0] - 'a', // 'a' -> 0, 'h' -> 7
        '8' - gridPosition[1]  // '8' -> 0, '1' -> 7
    };
}

// Returns the path of a chess piece image from it's piece information (Type and team)
static std::filesystem::path findImagePath(PieceInfo pieceInfo, const std::filesystem::path& projectPath) {
    if (pieceInfo.team == PieceTeam::White) {
        switch (pieceInfo.type) {
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
        switch (pieceInfo.type) {
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

// Draws a piece on the board
void ChessPiece::draw(PieceInfo pieceInfo, Vector2 boardPosition, Vector2 tileSize,
    Painter& painter, TextureCache& textureCache, const std::filesystem::path& projectPath) {

    float scale{0.75f};
    float heightDisplacement{0.25f};
    Vector2 spriteSize{tileSize.x * scale, tileSize.y * 2 * scale};

    Vector2 physicalPosition = {
        boardPosition.x + ((float)pieceInfo.position.x - (float)pieceInfo.position.y)
        * tileSize.x / 2.0f - spriteSize.y / 2.0f,
        boardPosition.y + ((float)pieceInfo.position.x + (float)pieceInfo.position.y)
        * tileSize.y / 2.0f - (spriteSize.y / 2.0f + heightDisplacement * spriteSize.y),
    };
    Texture& pieceTexture = textureCache.loadTexture(findImagePath(pieceInfo, projectPath).string());

    painter.drawSprite(
        physicalPosition,
        spriteSize,
        pieceTexture
    );
}
