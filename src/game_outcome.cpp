// Implementation for game_outcome.h
#include "game_outcome.h"
#include "move_generation.h"

// Checks for checkmate and stalemate after each move. (By order of priority)
Outcome game_outcome::getOutcome(const BoardState& boardState) {
    // Default game outcome is playing
    Outcome gameOutcome{Outcome::Playing};

    if (!hasLegalMoves(boardState))
        // There are no more legal moves available and the king is in check. This is a Checkmate.
        if (isKingInCheck(boardState)) {
            gameOutcome = (boardState.playerToMove == TeamColor::White)
                ? Outcome::BlackVictoryCheckmate
                : Outcome::WhiteVictoryCheckmate;
        }
    // There are no more legal moves available but the king is not in check. This is a stalemate.
        else {
            gameOutcome = Outcome::DrawStalemate;
        }
    // There are still legal moves available, but this could still be a draw by repetition or insufficient material.
    else {
        if (hasInsufficientMaterial(boardState)) {
            gameOutcome = Outcome::DrawInsufficientMaterial;
        }
        // 75 Move Rule (Automatic Draw)
        if (boardState.moveRuleCounter == 75) {
            gameOutcome = Outcome::Draw75Move;
        }
    }
    return gameOutcome;
}

// Returns true if the given team's king is currently being attacked.
bool game_outcome::isKingInCheck(const BoardState& boardState) {
    TeamColor team{boardState.playerToMove};

    // Find the king's position
    Vector2Int kingPos{-1, -1};
    for (int rank = 0; rank < 8; rank++)
        for (int file = 0; file < 8; file++)
            if (boardState.tiles[file][rank].type == PieceType::King && boardState.tiles[file][rank].team == team)
                kingPos = {file, rank};

    // Get squares attacked by the opponent (ignoreKing = false, we want accurate attacks)
    auto attacked = move_generation::getAttackedSquares(false, team, boardState.tiles, boardState.castlingRights);
    return attacked[kingPos.x][kingPos.y];
}

// Returns true if the given team has at least one legal move available.
bool game_outcome::hasLegalMoves(const BoardState& boardState) {
    TeamColor team{boardState.playerToMove};

    // Find pieces of the playing team
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if (boardState.tiles[file][rank].team != team) continue;

            Tile piece = boardState.tiles[file][rank];
            Vector2Int pos{file, rank};

            auto moves = move_generation::generateMovesForPiece(piece, pos, boardState.tiles, boardState.castlingRights, boardState.enPassantTargetSquare);
            move_generation::findLegalMovesForPiece(moves, piece, pos, boardState.tiles, boardState.castlingRights, boardState.enPassantTargetSquare);

            for (int r = 0; r < 8; r++)
                for (int f = 0; f < 8; f++)
                    if (moves[f][r] != MoveType::None)
                        return true;
        }
    }
    return false;
}

TileColor game_outcome::getTileColor(const Vector2Int& position) {
    return ((position.x + position.y) % 2 == 0) ? TileColor::Light : TileColor::Dark;
}

bool game_outcome::hasInsufficientMaterial(const BoardState& boardState) {
    // Find piece count in array
    auto getPieceCount = [](const auto& m, PieceType t) -> int {
        auto it = m.find(t);
        return it != m.end() ? it->second : 0;
        };

    // King vs King
    if (boardState.pieces.size() == 2) return true;

    // 3 Total Pieces
    if (boardState.pieces.size() == 3) {
        // King and Knight vs King
        if (std::count_if(boardState.pieces.begin(), boardState.pieces.end(), [](const Piece& e) {
            return e.type == PieceType::Knight;}) == 1) return true;
        // King and Bishop vs King
        if (std::count_if(boardState.pieces.begin(), boardState.pieces.end(), [](const Piece& e) {
            return e.type == PieceType::Bishop;}) == 1) return true;
    }

    // King and Bishop vs King and Bishop (Both Bishops must be in the same colour square)
    if (boardState.pieces.size() == 4) {

        auto whiteBishop = std::find_if(boardState.pieces.begin(), boardState.pieces.end(), [](const Piece& e) {
            return e.type == PieceType::Bishop && e.team == TeamColor::White;});

        auto blackBishop = std::find_if(boardState.pieces.begin(), boardState.pieces.end(), [](const Piece& e) {
            return e.type == PieceType::Bishop && e.team == TeamColor::Black;});

        // Check if both bishops exist
        if (whiteBishop != boardState.pieces.end() && blackBishop != boardState.pieces.end()
            && getTileColor(whiteBishop->position) == getTileColor(blackBishop->position)) {
            return true;
        }
    }
    return false;
}