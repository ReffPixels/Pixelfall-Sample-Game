// Implementation for game_state.h

#include "game_state.h"
#include "move_generation.h"
// Standard Library
#include <iostream>
#include <algorithm>

void GameState::setupFromFEN() {
    // Get FEN of the initial position
    FenState fenState{FenParser::getState(currentBoardFEN)};

    // Assign FEN values to the board state
    boardState.pieces = fenState.pieceList;
    boardState.playerToMove = fenState.playerToMove;
    boardState.castlingRights = fenState.castlingRights;
    boardState.enPassantTargetSquare = fenState.enPassantTargetSquare;
    boardState.moveRuleCounter = fenState.moveRuleCounter;
    boardState.totalFullMoves = fenState.totalFullMoves;

    // We have to manually sync the tileState since FEN only provides us with the pieceState
    syncTileState();
}

// Moves a piece from an origin square to a target square and applies rules based on the type of move.
void GameState::movePiece(Vector2Int origin, Vector2Int target, MoveType moveType) {
    Tile& selected = boardState.tiles[origin.x][origin.y];
    Tile& targetPiece = boardState.tiles[target.x][target.y];

    // Reset enPassant availability
    boardState.enPassantTargetSquare = {-1, -1};

    // Reset move rule counter if this was a capture or a pawn move
    if (targetPiece.type != PieceType::None || selected.type == PieceType::Pawn) boardState.moveRuleCounter = 0;
    else boardState.moveRuleCounter++;

    // Move piece
    targetPiece = selected;
    lastMove.origin = origin;
    lastMove.target = target;

    // Was it a double pawn push? Record en passant square.
    if (selected.type == PieceType::Pawn && abs(target.y - origin.y) == 2)
        boardState.enPassantTargetSquare = {target.x, (origin.y + target.y) / 2};

    // Was it en passant? Remove the captured pawn.
    if (moveType == MoveType::EnPassant)
        boardState.tiles[target.x][origin.y] = {PieceType::None, TeamColor::None};

    // Was it a promotion?
    if (moveType == MoveType::Promotion || moveType == MoveType::CapturePromotion) {
        // inputState = InputState::Promotion; [TODO SOON] Return movetype?
        promotionPosition = target;

        // Remove piece from its previous position
        selected = {PieceType::None, TeamColor::None};
        return;
    }

    // Was it castling? Move the rook to its new position
    if (selected.team == TeamColor::White) {
        if (moveType == MoveType::CastlingKingSide) {
            boardState.tiles[7][7] = {PieceType::None, TeamColor::None};
            boardState.tiles[5][7] = {PieceType::Rook, TeamColor::White};
        }
        else if (moveType == MoveType::CastlingQueenSide) {
            boardState.tiles[0][7] = {PieceType::None, TeamColor::None};
            boardState.tiles[3][7] = {PieceType::Rook, TeamColor::White};
        }
    }
    else if (selected.team == TeamColor::Black) {
        if (moveType == MoveType::CastlingKingSide) {
            boardState.tiles[7][0] = {PieceType::None, TeamColor::None};
            boardState.tiles[5][0] = {PieceType::Rook, TeamColor::Black};
        }
        else if (moveType == MoveType::CastlingQueenSide) {
            boardState.tiles[0][0] = {PieceType::None, TeamColor::None};
            boardState.tiles[3][0] = {PieceType::Rook, TeamColor::Black};
        }
    }

    // Remove piece from its previous position
    selected = {PieceType::None, TeamColor::None};
}

// Update Castling Rights (Must be called at the end of a move, since it uses lastMove.origin and lastMove.target)
void GameState::updateCastlingRights() {
    CastlingRights cr = boardState.castlingRights;
    // White King-side Rook
    if (cr.whiteKingSide &&
        ((lastMove.origin == Vector2Int{7, 7}) || (lastMove.target == Vector2Int{7, 7}))) {
        cr.whiteKingSide = false;
    }
    // White Queen-side Rook
    if (cr.whiteQueenSide &&
        ((lastMove.origin == Vector2Int{0, 7}) || (lastMove.target == Vector2Int{0, 7}))) {
        cr.whiteQueenSide = false;
    }
    // White King
    if ((cr.whiteKingSide || cr.whiteQueenSide) && lastMove.origin == Vector2Int{4, 7}) {
        cr.whiteKingSide = false;
        cr.whiteQueenSide = false;
    }
    // Black King-side Rook
    if (cr.blackKingSide &&
        ((lastMove.origin == Vector2Int{7, 0}) || (lastMove.target == Vector2Int{7, 0}))) {
        cr.blackKingSide = false;
    }
    // Black Queen-side Rook
    if (cr.blackQueenSide &&
        ((lastMove.origin == Vector2Int{0, 0}) || (lastMove.target == Vector2Int{0, 0}))) {
        cr.blackQueenSide = false;
    }
    // Black King
    if ((cr.blackKingSide || cr.blackQueenSide) && lastMove.origin == Vector2Int{4, 0}) {
        cr.blackKingSide = false;
        cr.blackQueenSide = false;
    }
    boardState.castlingRights = cr;
}

TeamColor GameState::getOpponent() {
    if (boardState.playerToMove == TeamColor::White) return TeamColor::Black;
    if (boardState.playerToMove == TeamColor::Black) return TeamColor::White;
    return TeamColor::None;
}

void GameState::swapPlayers() {
    boardState.playerToMove = getOpponent();
}

void GameState::incrementTotalMoves() {
    if (boardState.playerToMove == TeamColor::Black) boardState.totalFullMoves++;
}

// Returns true if the given team's king is currently being attacked.
bool GameState::isKingInCheck(TeamColor team) const {
    // Find the king's position
    Vector2Int kingPos{-1, -1};
    for (int rank = 0; rank < 8; rank++)
        for (int file = 0; file < 8; file++)
            if (boardState.tiles[file][rank].type == PieceType::King && boardState.tiles[file][rank].team == team)
                kingPos = {file, rank};

    // Get squares attacked by the opponent (ignoreKing = false, we want accurate attacks)
    auto attacked = MoveGeneration::getAttackedSquares(false, team, boardState.tiles, boardState.castlingRights);
    return attacked[kingPos.x][kingPos.y];
}

// Returns true if the given team has at least one legal move available.
bool GameState::hasLegalMoves(TeamColor team) const {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if (boardState.tiles[file][rank].team != team) continue;

            Tile piece = boardState.tiles[file][rank];
            Vector2Int pos{file, rank};

            auto moves = MoveGeneration::generateMovesForPiece(piece, pos, boardState.tiles, boardState.castlingRights, boardState.enPassantTargetSquare);
            MoveGeneration::findLegalMovesForPiece(moves, piece, pos, boardState.tiles, boardState.castlingRights, boardState.enPassantTargetSquare);

            for (int r = 0; r < 8; r++)
                for (int f = 0; f < 8; f++)
                    if (moves[f][r] != MoveType::None)
                        return true;
        }
    }
    return false;
}

// Checks for checkmate and stalemate after each move. (By order of priority)
bool GameState::findGameOutcome() {
    if (!hasLegalMoves(boardState.playerToMove))
        // There are no more legal moves available and the king is in check. This is a Checkmate.
        if (isKingInCheck(boardState.playerToMove)) {
            gameOutcome = (boardState.playerToMove == TeamColor::White)
                ? GameOutcome::BlackVictoryCheckmate
                : GameOutcome::WhiteVictoryCheckmate;
            return true;
        }
        // There are no more legal moves available but the king is not in check. This is a stalemate.
        else {
            gameOutcome = GameOutcome::DrawStalemate;
            return true;
        }
    // There are still legal moves available, but this could still be a draw by repetition or insufficient material.
    else {
        if (hasInsufficientMaterial()) {
            gameOutcome = GameOutcome::DrawInsufficientMaterial;
            return true;
        }
        // 75 Move Rule (Automatic Draw)
        if (boardState.moveRuleCounter == 75) {
            gameOutcome = GameOutcome::Draw75Move;
            return true;
        }
    }
    return false;
}

void GameState::removePiece(Vector2Int position) {
    boardState.tiles[position.x][position.y] = {PieceType::None, TeamColor::None};
}

void GameState::syncPieceState() {
    boardState.pieces.clear();

    for (int r = 0; r < 8; r++)
        for (int f = 0; f < 8; f++)
            if (boardState.tiles[f][r].type != PieceType::None)
                boardState.pieces.push_back(
                    {boardState.tiles[f][r].type, boardState.tiles[f][r].team, Vector2Int(f, r)});
}

void GameState::syncTileState() {
    for (auto& file : boardState.tiles)
        for (auto& tile : file)
            tile = {PieceType::None, TeamColor::None};

    for (const auto& piece : boardState.pieces)
        boardState.tiles[piece.position.x][piece.position.y] = {piece.type, piece.team};
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

void GameState::onPromotionSelected(PieceType pieceType) {
    boardState.tiles[promotionPosition.x][promotionPosition.y] = {pieceType, boardState.playerToMove};
}

void GameState::clearState() {
    // Reset Board State
    boardState.playerToMove = TeamColor::White;
    boardState.castlingRights = {true, true, true, true};
    boardState.enPassantTargetSquare = {-1, -1};
    boardState.moveRuleCounter = 0;
    boardState.totalFullMoves = 1;
    lastMove = {{-1,-1},{-1,-1}};

    // Reset other trackers
    gameOutcome = GameOutcome::Playing;
    promotionPosition = {-1, -1};
}