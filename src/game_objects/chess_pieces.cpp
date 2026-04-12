// Implementation for chess_pieces.h

#include "game_objects/chess_pieces.h"

// Draws all the pieces in the pieces vector on the screen.
void ChessPieces::drawPieces(std::vector<Piece>& pieces, ChessBoard& board, Painter& painter) {

    for (auto& piece : pieces)
        drawPiece(piece, board, painter);
};


// Draws a piece on the board
void ChessPieces::drawPiece(Piece& piece, ChessBoard& board, Painter& painter) {

    // Check that this is a valid piece
    if ((piece.type == PieceType::None) || (piece.team == TeamColor::None)) return;

    // Calculate the final position on the screen
    Vector2 physicalPosition{
            board.getPosition().x
            + (float)piece.position.x
            * board.getTileSize().x
            + pieceOffset.x,

            board.getPosition().y
            + (float)board.getRankByDirection(piece.position.y) // Check if the board is flipped
            * board.getTileSize().y
            + pieceOffset.y,
    };

    // Create a texture with the correct image depending on the piece type and team.
    Texture& pieceTexture = painter.textureCache->loadTexture(findImagePath(piece.type, piece.team).string());

    painter.drawSprite(
        physicalPosition,
        spriteSize,
        pieceTexture
    );
}

// Draws a piece floating in a given position
void ChessPieces::drawFree(PieceType type, TeamColor team, Vector2 position, Painter& painter, Vector2 size) {

    // Check that this is a valid piece
    if ((type == PieceType::None) || (team == TeamColor::None)) return;

    // Create a texture with the correct image depending on the piece type and team.
    Texture& pieceTexture = painter.textureCache->loadTexture(findImagePath(type, team).string());

    // If the sprite size was not provided (Size 0, 0) default to the normal pieces's sprite size
    if (size == Vector2::Zero) size = spriteSize;

    painter.drawSprite(
        position,
        size,
        pieceTexture
    );
}

// Returns the path of a chess piece image from it's piece information (Type and team)
std::filesystem::path ChessPieces::findImagePath(PieceType type, TeamColor team) {
    if (team == TeamColor::White) {
        switch (type) {
        case PieceType::King: return (spritesPath / "white_king.png");
        case PieceType::Queen: return (spritesPath / "white_queen.png");
        case PieceType::Rook: return (spritesPath / "white_rook.png");
        case PieceType::Bishop: return (spritesPath / "white_bishop.png");
        case PieceType::Knight: return (spritesPath / "white_knight.png");
        case PieceType::Pawn: return (spritesPath / "white_pawn.png");
        default: throw std::runtime_error("Invalid piece type"); // [Error]
        }
    }
    else {
        switch (type) {
        case PieceType::King: return (spritesPath / "black_king.png");
        case PieceType::Queen: return (spritesPath / "black_queen.png");
        case PieceType::Rook: return (spritesPath / "black_rook.png");
        case PieceType::Bishop: return (spritesPath / "black_bishop.png");
        case PieceType::Knight: return (spritesPath / "black_knight.png");
        case PieceType::Pawn: return (spritesPath / "black_pawn.png");
        default: throw std::runtime_error("Invalid piece type"); // [Error]
        }
    }
}

// Draw a piece attached to the cursor
void ChessPieces::pieceFollowCursor(Vector2& cursorPos, ChessBoard& board,
    PieceType type, TeamColor team, Painter& painter, Vector2 offset) {

    // Clamp cursor position to inside the board
    Vector2 positionInBoard{
        std::clamp(cursorPos.x,
            board.getPosition().x, board.getPosition().x + board.getTileSize().x * 8),
        std::clamp(cursorPos.y,
            board.getPosition().y, board.getPosition().y + board.getTileSize().y * 8)
    };

    drawFree(type, team, positionInBoard + offset, painter, spriteSize);
}

// Computes the pivot offset so the dragged piece stays attached at the grab point
Vector2 ChessPieces::computeDragPivot(Vector2& cursorPos, ChessBoard& board) {
    Vector2Int piecePosition = board.getTileOnHover(cursorPos);
    return {cursorPos - (board.getPosition() +
        Vector2{(float)piecePosition.x, (float)piecePosition.y} * board.getTileSize())};
}