// Implementation for piece.h
#include "chess_pieces.h"
#include "config/board_config.h"

// Converts a piece location from grid notation (a1 to h8) into a vector.
Vector2Int ChessPieces::getPosFromNotation(std::string gridPosition) {
    return Vector2Int{
        gridPosition[0] - 'a', // 'a' -> 0, 'h' -> 7
        '8' - gridPosition[1]  // '8' -> 0, '1' -> 7
    };
}

// Converts a piece vector location into grid notation (a1 to h8).
std::string ChessPieces::getNotationFromPos(Vector2Int gridPosition) {
    return std::string() +
        char('a' + gridPosition.x) +
        char('8' + gridPosition.y);
}

// Returns the path of a chess piece image from it's piece information (Type and team)
static std::filesystem::path findImagePath(PieceInfo pieceInfo) {
    if (pieceInfo.team == PieceTeam::White) {
        switch (pieceInfo.type) {
        case King: return (board::piecesPath / "white_king.png");
        case Queen: return (board::piecesPath / "white_queen.png");
        case Rook: return (board::piecesPath / "white_rook.png");
        case Bishop: return (board::piecesPath / "white_bishop.png");
        case Knight: return (board::piecesPath / "white_knight.png");
        case Pawn: return (board::piecesPath / "white_pawn.png");
        default: return NULL; // Error, invalid piece type
        }
    }
    else {
        switch (pieceInfo.type) {
        case King: return (board::piecesPath / "black_king.png");
        case Queen: return (board::piecesPath / "black_queen.png");
        case Rook: return (board::piecesPath / "black_rook.png");
        case Bishop: return (board::piecesPath / "black_bishop.png");
        case Knight: return (board::piecesPath / "black_knight.png");
        case Pawn: return (board::piecesPath / "black_pawn.png");
        default: return NULL; // Error, invalid piece type
        }
    }
}

// Draws a piece on the board
void ChessPieces::draw(PieceInfo pieceInfo, Vector2 boardPosition, Vector2 tileSize, Vector2 spriteSize,
    Painter& painter,
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
    
    Texture& pieceTexture = painter.textureCache->loadTexture(findImagePath(pieceInfo).string());

    painter.drawSprite(
        physicalPosition,
        spriteSize,
        pieceTexture
    );
}


// Draws a piece floating in a giving position
void ChessPieces::drawFree(PieceType type, PieceTeam team, Vector2 physicalPosition, Vector2 spriteSize,
    Painter& painter) {

    Texture& pieceTexture = painter.textureCache->loadTexture(findImagePath({type, team, Vector2Int::Zero}).string());

    painter.drawSprite(
        physicalPosition,
        spriteSize,
        pieceTexture
    );
}

// Draws all the pieces in the pieces vector on the screen.
void ChessPieces::drawPieces(std::vector<PieceInfo> pieces, Vector2 boardPosition, Vector2 tileSize, Vector2 spriteSize,
    Painter& painter,
    Vector2 pieceOffset) {

    for (PieceInfo pieceInfo : pieces) {
        ChessPieces piece;
        piece.draw(pieceInfo, boardPosition, tileSize, spriteSize, painter, pieceOffset);
    }
};