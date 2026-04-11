// Implementation for chess_state.h

#include "chess_state.h"
#include "chess_moves.h"
// Standard Library
#include <iostream>
#include <algorithm>

void ChessState::setupFromFEN() {
    boardStatus = fenParser.getBoardFromFEN(currentBoardFEN);
}

// Pressing allows to select a new piece or move if a piece is already selected.
void ChessState::onBoardPressed(Vector2Int square) {
    PieceInfo& clicked = boardState[square.x][square.y];
    if (inputState == InputState::Promotion) return;

    // It's a piece in the player's team
    if (clicked.type != PieceType::None && clicked.team == playerToMove)
        // Clicked on the same square, deselect
        if (square == selPiecePosition) {
            deselectPiece();
            return;
        } else selectPiece(square);
    else
        moveSelectedPiece(square);
}

// Release only allows to move if a piece is already selected. (Drag and drop behaviour) Otherwise it does nothing.
void ChessState::onBoardReleased(Vector2Int square) {
    if (inputState == InputState::Promotion) return;

    moveSelectedPiece(square);
}

// Selects a piece in a specific square and changes the input state to selected.
void ChessState::selectPiece(Vector2Int selectedSquare) {
    selPiecePosition = selectedSquare;
    PieceInfo& selPieceInfo = boardState[selPiecePosition.x][selPiecePosition.y];
    inputState = InputState::PieceSelected;
    validMoves = ChessMoves::generateMovesForPiece(
        selPieceInfo, selPiecePosition, boardState, castlingRights, enPassantTargetSquare);
    ChessMoves::findLegalMovesForPiece(
        validMoves, selPieceInfo, selPiecePosition, boardState, castlingRights, enPassantTargetSquare);
}

// Moves the selected piece to a new square and updates the board state to match. 
// A succesful move triggers the next turn.
void ChessState::moveSelectedPiece(Vector2Int targetSquare) {
    MoveType moveType = validMoves[targetSquare.x][targetSquare.y];

    if (inputState == InputState::Normal) return;
    if (targetSquare == selPiecePosition) return;
    if (moveType == MoveType::None) {
        deselectPiece();
        return;
    }

    movePiece(selPiecePosition, targetSquare, moveType);

    if (inputState != InputState::Promotion) {
        nextTurn();
    }
}

// Moves a piece from an origin square to a target square and applies rules based on the type of move.
void ChessState::movePiece(Vector2Int origin, Vector2Int target, MoveType moveType) {
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


// Swaps the current player and runs necessary functions to set up the next turn (Or end the game)
void ChessState::nextTurn() {
    // Swap player
    playerToMove = (playerToMove == TeamColor::White ? TeamColor::Black : TeamColor::White);
    // Count full moves
    if (playerToMove == TeamColor::Black) totalFullMoves++;

    deselectPiece();
    updateCastlingRights();
    updatePieceList();
    printStatistics();
    findGameOutcome();
}

// Update Castling Rights (Must be called at the end of a move, since it uses lastMoveOrigin and lastMoveTarget)
void ChessState::updateCastlingRights() {
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

// Reset selected piece trackers
void ChessState::deselectPiece() {
    selPiecePosition = {-1, -1};
    inputState = InputState::Normal;
    ChessMoves::clearMoves(validMoves);
}

// Find all of the squares attacked by the oponent. 
// If ignoreKing is set to true, the attacked squares of sliding pieces will ignore the king.
// This is necessary for finding safe king squares since the king cannot retreat to the squares it was blocking. (X-ray)
std::array<std::array<bool, 8>, 8> ChessState::getAttackedSquares(bool ignoreKing, TeamColor playerTeam,
    const std::array<std::array<PieceInfo, 8>, 8>& boardState, const CastlingRights& castlingRights) {
    // Store attacked squares
    std::array<std::array<bool, 8>, 8> attackedSquares;

    // Set attacked squares to none
    for (auto& rank : attackedSquares)
        for (auto& square : rank)
            square = false;

    // Store an adjusted board state (In case we are ignoring the king)
    std::array<std::array<PieceInfo, 8>, 8> adjustedBoardState = boardState;
    if (ignoreKing) {
        for (int r = 0; r < 8; r++)
            for (int f = 0; f < 8; f++)
                if (boardState[f][r].team == playerTeam && boardState[f][r].type == PieceType::King)
                    adjustedBoardState[f][r] = {PieceType::None, TeamColor::None};
    }

    // Find every enemy piece
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            PieceInfo& cell = adjustedBoardState[file][rank];
            // This cell is empty, do nothing
            if (cell.type == PieceType::None || cell.team == playerTeam)
                continue; 

            // This cell is a pawn. Pawns attack diagonally so they require special treatment
            // We don't need an exception for en passant since it doesn't change the attack shape (Diagonal)
            if (cell.type == PieceType::Pawn) {
                int direction = (cell.team == TeamColor::White) ? -1 : 1;
                int attackRank = rank + direction;
                if (attackRank >= 0 && attackRank < 8) {
                    if (file - 1 >= 0) attackedSquares[file - 1][attackRank] = true;
                    if (file + 1 < 8)  attackedSquares[file + 1][attackRank] = true;
                }
                continue;
            }

            // This cell is a piece other than a pawn, we can use move generation to find its attack squares
            std::array<std::array<MoveType, 8>, 8> moves =
                ChessMoves::generateMovesForPiece(cell, Vector2Int(file, rank), adjustedBoardState, castlingRights);

            // Only add valid attack moves to the attacked squares (Not Empty or Castling)
            // We don't need to account for promotion since forward pawn moves don't attack anyway.
            for (int r = 0; r < 8; r++) {
                for (int f = 0; f < 8; f++) {
                    MoveType m = moves[f][r];
                    if (m == MoveType::None
                        || m == MoveType::CastlingKingSide
                        || m == MoveType::CastlingQueenSide)
                        continue;

                    attackedSquares[f][r] = true;
                }
            }
        }
    }

    return attackedSquares;
}

TeamColor ChessState::getOpponent() {
    if (playerToMove == TeamColor::White) return TeamColor::Black;
    if (playerToMove == TeamColor::Black) return TeamColor::White;
    return TeamColor::None;
}

void ChessState::endGame() {
    std::cout << "-------------------------------" << std::endl;
    switch (gameOutcome) {
    case GameOutcome::WhiteVictoryCheckmate: std::cout << "WHITE VICTORY (CHECKMATE)" << std::endl; break;
    case GameOutcome::BlackVictoryCheckmate: std::cout << "BLACK VICTORY (CHECKMATE)" << std::endl; break;
    case GameOutcome::WhiteVictoryResignation: std::cout << "WHITE VICTORY (RESIGNATION)" << std::endl; break;
    case GameOutcome::BlackVictoryResignation: std::cout << "BLACK VICTORY (RESIGNATION)" << std::endl; break;
    case GameOutcome::WhiteVictoryTimeout: std::cout << "WHITE VICTORY (TIMEOUT)" << std::endl; break;
    case GameOutcome::BlackVictoryTimeout: std::cout << "BLACK VICTORY (TIMEOUT)" << std::endl; break;
    case GameOutcome::DrawStalemate: std::cout << "DRAW (STALEMATE)" << std::endl; break;
    case GameOutcome::DrawInsufficientMaterial: std::cout << "DRAW (INSUFFICIENT MATERIAL)" << std::endl; break;
    case GameOutcome::Draw50Move: std::cout << "DRAW (50 MOVE RULE)" << std::endl; break;
    case GameOutcome::Draw75Move: std::cout << "DRAW (75 MOVE RULE)" << std::endl; break;
    case GameOutcome::Draw3FoldRepetition: std::cout << "DRAW (3 FOLD REPETITION)" << std::endl; break;
    case GameOutcome::Draw5FoldRepetition: std::cout << "DRAW (5 FOLD REPETITION)" << std::endl; break;
    case GameOutcome::DrawAgreement: std::cout << "DRAW (AGREEMENT)" << std::endl; break;
    }
}

void ChessState::resetGame() {
    // White always goes first, even if we are playing from black's perspective
    playerToMove = TeamColor::White;
    
    // Reset board state and trackers
    gameOutcome = GameOutcome::Playing;
    castlingRights = {true, true, true, true};
    enPassantTargetSquare = {-1, -1};
    moveRuleCounter = 0;
    totalFullMoves = 1;
    lastMoveOrigin = {-1, -1};
    lastMoveTarget = {-1, -1};
    setupFromFEN();

    printStatistics();
}

// Returns true if the given team's king is currently being attacked.
bool ChessState::isKingInCheck(TeamColor team) const {
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
bool ChessState::hasLegalMoves(TeamColor team) const {
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
void ChessState::findGameOutcome() {

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

void ChessState::removePiece(Vector2Int position) {
    boardState[position.x][position.y] = {PieceType::None, TeamColor::None};
}

void ChessState::updatePieceList() {
    pieceList.clear();

    for (int r = 0; r < 8; r++)
        for (int f = 0; f < 8; f++)
            if (boardState[f][r].type != PieceType::None)
                pieceList.push_back({boardState[f][r], Vector2Int(f, r)});
}

static SquareColor getSquareColor(Vector2Int position) {
    return ((position.x + position.y) % 2 == 0) ? SquareColor::Light : SquareColor::Dark;
}

bool ChessState::hasInsufficientMaterial() {
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
            && getSquareColor(whiteBishop->position) == getSquareColor(blackBishop->position)) {
            return true;
        }
    }
    return false;
}

void ChessState::printStatistics() {
    std::cout << "-------------------------------" << '\n';
    std::cout << "Player to Move: " << static_cast<int>(playerToMove) << '\n';
    std::cout << "Half Moves: " << moveRuleCounter << '\n';
    std::cout << "Full Moves: " << totalFullMoves << '\n';
    std::cout << "Castling Rights KQkq: " << castlingRights.whiteKingSide << castlingRights.whiteQueenSide
        << castlingRights.blackKingSide << castlingRights.blackQueenSide << std::endl;
}

void ChessState::onPromotionSelected(PieceType pieceType) {
    boardState[promotionPosition.x][promotionPosition.y] = {pieceType, playerToMove};
    nextTurn();
}