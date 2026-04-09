// This is a FEN parser that reads FEN strings and can populate the board to match them.
// FEN (Forsyth–Edwards Notation) is a standard notation for describing the position of the pieces in a chess game.
// Default position is rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1

#pragma once
// Chess
#include "chess_pieces.h"
// Standard Library
#include <string>
#include <map>
#include <array>

class FenParser {
public:
    // Methods
    std::array<std::array<PieceInfo, 8>, 8> getBoardFromFEN(const std::string fenString);

private:
    std::map<char, PieceInfo> pieceCodes = {
        // White
        {'K', {PieceType::King, PieceTeam::White}},
        {'Q', {PieceType::Queen, PieceTeam::White}},
        {'R', {PieceType::Rook, PieceTeam::White}},
        {'B', {PieceType::Bishop, PieceTeam::White}},
        {'N', {PieceType::Knight, PieceTeam::White}},
        {'P', {PieceType::Pawn, PieceTeam::White}},
        // Black
        {'k', {PieceType::King, PieceTeam::Black}},
        {'q', {PieceType::Queen, PieceTeam::Black}},
        {'r', {PieceType::Rook, PieceTeam::Black}},
        {'b', {PieceType::Bishop, PieceTeam::Black}},
        {'n', {PieceType::Knight, PieceTeam::Black}},
        {'p', {PieceType::Pawn, PieceTeam::Black}}
    };
};

// [TODO] Opposite getter, Update FEN string based on array