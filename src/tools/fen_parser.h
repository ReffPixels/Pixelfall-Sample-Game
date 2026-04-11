// This is a FEN parser that reads FEN strings and can populate the board to match them.
// FEN (Forsyth–Edwards Notation) is a standard notation for describing the position of the pieces in a chess game.
// Default position is rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1

#pragma once
// Chess
#include "config/chess_config.h"
// Standard Library
#include <string>
#include <vector>

// FEN string translated to game data
struct FenState {
    std::vector<Piece> pieceList;
    TeamColor playerToMove{TeamColor::White}; // Player that gets to move next.
    CastlingRights castlingRights{false, false, false, false}; // Tracker for who's lost their castling rights.
    Vector2Int enPassantTargetSquare{-1, -1}; // Record where en passant is possible (Cannot be deduced from positions)
    int moveRuleCounter{0}; // Used for 50 fold and 75 fold repetition. Counts on every pawn move or capture.
    int totalFullMoves{1}; // Starts at 1 due to some arcaic reason. Counts up only on black moves.
};

// Utility functions for parsing FEN strings
namespace FenParser {
    FenState getState(const std::string& fenString);
    Piece getPieceFromSymbol(char symbol, Vector2Int position);
};