// Implementation for game_state.h

#include "game_state.h"
#include "move_generation.h"
// Standard Library
#include <iostream>
#include <algorithm>

void GameState::setupFromFEN() {
    boardStatus = fenParser.getBoardFromFEN(currentBoardFEN);
}

// Moves a piece from an origin square to a target square and applies rules based on the type of move.
void GameState::movePiece(Vector2Int origin, Vector2Int target, MoveType moveType) {
    PieceInfo& selected = boardState[origin.x][origin.y];
    PieceInfo& targetPiece = boardState[target.x][target.y];

    // Reset enPassant availability
    enPassantTargetSquare = {-1, -1};

    // Reset move rule counter if this was a capture or a pawn move
    if (targetPiece.type != PieceType::None || selected.type == PieceType::Pawn) moveRuleCounter = 0;
    else moveRuleCounter++;

    // Move piece
    targetPiece = selected;
    lastMoveOrigin = origin;
    lastMoveTarget = target;

    // Was it a double pawn push? Record en passant square.
    if (selected.type == PieceType::Pawn && abs(target.y - origin.y) == 2)
        enPassantTargetSquare = {target.x, (origin.y + target.y) / 2};

    // Was it en passant? Remove the captured pawn.
    if (moveType == MoveType::EnPassant)
        boardState[target.x][origin.y] = {PieceType::None, TeamColor::None};

    // Was it a promotion?
    if (moveType == MoveType::Promotion || moveType == MoveType::CapturePromotion) {
        inputState = InputState::Promotion;
        promotionPosition = target;

        // Remove piece from its previous position
        selected = {PieceType::None, TeamColor::None};
        return;
    }

    // Was it castling? Move the rook to its new position
    if (selected.team == TeamColor::White) {
        if (moveType == MoveType::CastlingKingSide) {
            boardState[7][7] = {PieceType::None, TeamColor::None};
            boardState[5][7] = {PieceType::Rook, TeamColor::White};
        }
        else if (moveType == MoveType::CastlingQueenSide) {
            boardState[0][7] = {PieceType::None, TeamColor::None};
            boardState[3][7] = {PieceType::Rook, TeamColor::White};
        }
    }
    else if (selected.team == TeamColor::Black) {
        if (moveType == MoveType::CastlingKingSide) {
            boardState[7][0] = {PieceType::None, TeamColor::None};
            boardState[5][0] = {PieceType::Rook, TeamColor::Black};
        }
        else if (moveType == MoveType::CastlingQueenSide) {
            boardState[0][0] = {PieceType::None, TeamColor::None};
            boardState[3][0] = {PieceType::Rook, TeamColor::Black};
        }
    }

    // Remove piece from its previous position
    selected = {PieceType::None, TeamColor::None};
}

// Update Castling Rights (Must be called at the end of a move, since it uses lastMoveOrigin and lastMoveTarget)
void GameState::updateCastlingRights() {
    // White King-side Rook
    if (castlingRights.whiteKingSide &&
        ((lastMoveOrigin == Vector2Int{7, 7}) || (lastMoveTarget == Vector2Int{7, 7}))) {
        castlingRights.whiteKingSide = false;
    }
    // White Queen-side Rook
    if (castlingRights.whiteQueenSide &&
        ((lastMoveOrigin == Vector2Int{0, 7}) || (lastMoveTarget == Vector2Int{0, 7}))) {
        castlingRights.whiteQueenSide = false;
    }
    // White King
    if ((castlingRights.whiteKingSide || castlingRights.whiteQueenSide) && lastMoveOrigin == Vector2Int{4, 7}) {
        castlingRights.whiteKingSide = false;
        castlingRights.whiteQueenSide = false;
    }
    // Black King-side Rook
    if (castlingRights.blackKingSide &&
        ((lastMoveOrigin == Vector2Int{7, 0}) || (lastMoveTarget == Vector2Int{7, 0}))) {
        castlingRights.blackKingSide = false;
    }
    // Black Queen-side Rook
    if (castlingRights.blackQueenSide &&
        ((lastMoveOrigin == Vector2Int{0, 0}) || (lastMoveTarget == Vector2Int{0, 0}))) {
        castlingRights.blackQueenSide = false;
    }
    // Black King
    if ((castlingRights.blackKingSide || castlingRights.blackQueenSide) && lastMoveOrigin == Vector2Int{4, 0}) {
        castlingRights.blackKingSide = false;
        castlingRights.blackQueenSide = false;
    }
}

TeamColor GameState::getOpponent() {
    if (playerToMove == TeamColor::White) return TeamColor::Black;
    if (playerToMove == TeamColor::Black) return TeamColor::White;
    return TeamColor::None;
}

// Returns true if the given team's king is currently being attacked.
bool GameState::isKingInCheck(TeamColor team) const {
    // Find the king's position
    Vector2Int kingPos{-1, -1};
    for (int rank = 0; rank < 8; rank++)
        for (int file = 0; file < 8; file++)
            if (boardState[file][rank].type == PieceType::King && boardState[file][rank].team == team)
                kingPos = {file, rank};

    // Get squares attacked by the opponent (ignoreKing = false, we want accurate attacks)
    auto attacked = getAttackedSquares(false, team, boardState, castlingRights);
    return attacked[kingPos.x][kingPos.y];
}

// Returns true if the given team has at least one legal move available.
bool GameState::hasLegalMoves(TeamColor team) const {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if (boardState[file][rank].team != team) continue;

            PieceInfo piece = boardState[file][rank];
            Vector2Int pos{file, rank};

            auto moves = ChessMoves::generateMovesForPiece(piece, pos, boardState, castlingRights, enPassantTargetSquare);
            ChessMoves::findLegalMovesForPiece(moves, piece, pos, boardState, castlingRights, enPassantTargetSquare);

            for (int r = 0; r < 8; r++)
                for (int f = 0; f < 8; f++)
                    if (moves[f][r] != MoveType::None)
                        return true;
        }
    }
    return false;
}

// Checks for checkmate and stalemate after each move. (By order of priority)
void GameState::findGameOutcome() {

    if (!hasLegalMoves(playerToMove))
        // There are no more legal moves available and the king is in check. This is a Checkmate.
        if (isKingInCheck(playerToMove)) {
            gameOutcome = (playerToMove == TeamColor::White)
                ? GameOutcome::BlackVictoryCheckmate
                : GameOutcome::WhiteVictoryCheckmate;
            endGame();
            return;
        }
        // There are no more legal moves available but the king is not in check. This is a stalemate.
        else {
            gameOutcome = GameOutcome::DrawStalemate;
            endGame();
            return;
        }
    // There are still legal moves available, but this could still be a draw by repetition or insufficient material.
    else {
        if (hasInsufficientMaterial()) {
            gameOutcome = GameOutcome::DrawInsufficientMaterial;
            endGame();
            return;
        }
        // 75 Move Rule (Automatic Draw)
        if (moveRuleCounter == 75) {
            gameOutcome = GameOutcome::Draw75Move;
            endGame();
            return;
        }
    }
}

void GameState::removePiece(Vector2Int position) {
    boardState[position.x][position.y] = {PieceType::None, TeamColor::None};
}

void GameState::updatePieceList() {
    pieceList.clear();

    for (int r = 0; r < 8; r++)
        for (int f = 0; f < 8; f++)
            if (boardState[f][r].type != PieceType::None)
                pieceList.push_back({boardState[f][r], Vector2Int(f, r)});
}

static TileColor getTileColor(Vector2Int position) {
    return ((position.x + position.y) % 2 == 0) ? TileColor::Light : TileColor::Dark;
}

bool GameState::hasInsufficientMaterial() {
    // Find piece count in array
    auto getPieceCount = [](const auto& m, PieceType t) -> int {
        auto it = m.find(t);
        return it != m.end() ? it->second : 0;
        };

    // King vs King
    if (pieceList.size() == 2) return true;

    // 3 Total Pieces
    if (pieceList.size() == 3) {
        // King and Knight vs King
        if (std::count_if(pieceList.begin(), pieceList.end(), [](const Piece& e) {
            return e.info.type == PieceType::Knight;}) == 1) return true;
        // King and Bishop vs King
        if (std::count_if(pieceList.begin(), pieceList.end(), [](const Piece& e) {
            return e.info.type == PieceType::Bishop;}) == 1) return true;
    }

    // King and Bishop vs King and Bishop (Both Bishops must be in the same colour square)
    if (pieceList.size() == 4) {

        auto whiteBishop = std::find_if(pieceList.begin(), pieceList.end(), [](const Piece& e) {
            return e.info.type == PieceType::Bishop && e.info.team == TeamColor::White;});

        auto blackBishop = std::find_if(pieceList.begin(), pieceList.end(), [](const Piece& e) {
            return e.info.type == PieceType::Bishop && e.info.team == TeamColor::Black;});

        // Check if both bishops exist
        if (whiteBishop != pieceList.end() && blackBishop != pieceList.end()
            && getTileColor(whiteBishop->position) == getTileColor(blackBishop->position)) {
            return true;
        }
    }
    return false;
}

void GameState::onPromotionSelected(PieceType pieceType) {
    boardState[promotionPosition.x][promotionPosition.y] = {pieceType, playerToMove};
    nextTurn();
}