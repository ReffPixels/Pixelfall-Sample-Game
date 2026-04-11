// Implementation for piece.h
#include "game_objects/chess_pieces.h"
#include "config/board_config.h"
#include <stdexcept>

// Returns the path of a chess piece image from it's piece information (Type and team)
static std::filesystem::path findImagePath(PieceInfo pieceInfo) {
    if (pieceInfo.team == TeamColor::White) {
        switch (pieceInfo.type) {
        case PieceType::King: return (board::piecesPath / "white_king.png");
        case PieceType::Queen: return (board::piecesPath / "white_queen.png");
        case PieceType::Rook: return (board::piecesPath / "white_rook.png");
        case PieceType::Bishop: return (board::piecesPath / "white_bishop.png");
        case PieceType::Knight: return (board::piecesPath / "white_knight.png");
        case PieceType::Pawn: return (board::piecesPath / "white_pawn.png");
        default: throw std::runtime_error("Invalid piece type"); // [Error]
        }
    }
    else {
        switch (pieceInfo.type) {
        case PieceType::King: return (board::piecesPath / "black_king.png");
        case PieceType::Queen: return (board::piecesPath / "black_queen.png");
        case PieceType::Rook: return (board::piecesPath / "black_rook.png");
        case PieceType::Bishop: return (board::piecesPath / "black_bishop.png");
        case PieceType::Knight: return (board::piecesPath / "black_knight.png");
        case PieceType::Pawn: return (board::piecesPath / "black_pawn.png");
        default: throw std::runtime_error("Invalid piece type"); // [Error]
        }
    }
}

// Draws a piece on the board
void ChessPieces::draw(PieceInfo pieceInfo, Vector2Int piecePosition, Vector2 boardPosition, Vector2 tileSize, Vector2 spriteSize,
    Painter& painter, Vector2 pieceOffset) {

    // Check that this is a valid piece
    if ((pieceInfo.type == PieceType::None) || (pieceInfo.team == TeamColor::None)) return;

    Vector2 physicalPosition{};
    if (board::projectionType == ThemeProjection::Isometric) {
        physicalPosition = {
            boardPosition.x
            + ((float)piecePosition.x - (float)piecePosition.y)
            * (tileSize.x / 2.0f)
            - (spriteSize.x / 2.0f)
            + pieceOffset.x,
                
            boardPosition.y
            + ((float)piecePosition.x + (float)piecePosition.y)
            * (tileSize.y / 2.0f)
            - (spriteSize.y / 2.0f)
            + pieceOffset.y,
        };
    }
    else {
        physicalPosition = {
            boardPosition.x
            + (float)piecePosition.x
            * tileSize.x
            + pieceOffset.x,

            boardPosition.y
            + (float)piecePosition.y
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


// Draws a piece floating in a given position
void ChessPieces::drawFree(PieceType type, TeamColor team, Vector2 physicalPosition, Vector2 spriteSize,
    Painter& painter) {

    // Check that this is a valid piece
    if ((type == PieceType::None) || (team == TeamColor::None)) return;

    Texture& pieceTexture = painter.textureCache->loadTexture(findImagePath({type, team}).string());

    painter.drawSprite(
        physicalPosition,
        spriteSize,
        pieceTexture
    );
}

// Draws all the pieces in the pieces vector on the screen.
void ChessPieces::drawPieces(std::array<std::array<PieceInfo, 8>, 8> boardState, Vector2 boardPosition, Vector2 tileSize, Vector2 spriteSize,
    Painter& painter, Vector2 pieceOffset, Vector2Int selPiecePosition) {

    for (int rank = 0; rank < 8; rank++)
        for (int file = 0; file < 8; file++)
            // Check if there's a piece in the square
            if (boardState[file][rank].type != PieceType::None) {
                if (hideSelectedPiece && Vector2Int{file, rank} == selPiecePosition) {
                    // Don't draw this piece
                    continue;
                }
                    draw(boardState[file][rank], {file, flipPieces ? 7 - rank : rank}, boardPosition, tileSize, spriteSize, painter, pieceOffset);
            }
};