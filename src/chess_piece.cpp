// Implementation for piece.h
#include "chess_piece.h"
#include "config/board_config.h"

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
        case King: return (projectPath / board::piecesPath / "white_king.png");
        case Queen: return (projectPath / board::piecesPath / "white_queen.png");
        case Rook: return (projectPath / board::piecesPath / "white_rook.png");
        case Bishop: return (projectPath / board::piecesPath / "white_bishop.png");
        case Knight: return (projectPath / board::piecesPath / "white_knight.png");
        case Pawn: return (projectPath / board::piecesPath / "white_pawn.png");
        default: return projectPath; // Error, invalid piece type
        }
    }
    else {
        switch (pieceInfo.type) {
        case King: return (projectPath / board::piecesPath / "black_king.png");
        case Queen: return (projectPath / board::piecesPath / "black_queen.png");
        case Rook: return (projectPath / board::piecesPath / "black_rook.png");
        case Bishop: return (projectPath / board::piecesPath / "black_bishop.png");
        case Knight: return (projectPath / board::piecesPath / "black_knight.png");
        case Pawn: return (projectPath / board::piecesPath / "black_pawn.png");
        default: return projectPath; // Error, invalid piece type
        }
    }
}

// Draws a piece on the board
void ChessPiece::draw(PieceInfo pieceInfo, Vector2 boardPosition, Vector2 tileSize, Vector2 spriteSize,
    Painter& painter, TextureCache& textureCache, const std::filesystem::path& projectPath,
    Vector2 pieceOffset) {
    
    Vector2 physicalPosition{};
    if (board::projectionType == ThemeProjection::Isometric) {
        physicalPosition = {
            boardPosition.x
            + ((float)pieceInfo.position.x - (float)pieceInfo.position.y)
            * (tileSize.x / 2.0f)
            - (spriteSize.x / 2.0f)
            + pieceOffset.x,
                
            boardPosition.y
            + ((float)pieceInfo.position.x + (float)pieceInfo.position.y)
            * (tileSize.y / 2.0f)
            - (spriteSize.y / 2.0f)
            + pieceOffset.y,
        };
    }
    else {
        physicalPosition = {
            boardPosition.x
            + (float)pieceInfo.position.x
            * tileSize.x
            + pieceOffset.x,

            boardPosition.y
            + (float)pieceInfo.position.y
            * tileSize.y
            + pieceOffset.y,
        };
    }
    
    Texture& pieceTexture = textureCache.loadTexture(findImagePath(pieceInfo, projectPath).string());

    painter.drawSprite(
        physicalPosition,
        spriteSize,
        pieceTexture
    );
}


// Draws a piece floating in a giving position
void ChessPiece::drawFree(PieceType type, PieceTeam team, Vector2 physicalPosition, Vector2 spriteSize,
    Painter& painter, TextureCache& textureCache, const std::filesystem::path& projectPath) {

    Texture& pieceTexture = textureCache.loadTexture(findImagePath({type, team, Vector2Int::Zero}, projectPath).string());

    painter.drawSprite(
        physicalPosition,
        spriteSize,
        pieceTexture
    );
}