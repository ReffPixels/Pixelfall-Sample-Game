// Implementation for tools/fen_parser.h

#include "tools/fen_parser.h"

std::array<std::array<PieceInfo, 8>, 8> FenParser::getBoardFromFEN(const std::string fenString) {
    // Create board state
    std::array<std::array<PieceInfo, 8>, 8> boardState{};

    // Reset all squares to None
    for (auto& rank : boardState)
        for (auto& cell : rank)
            cell = {PieceType::None, PieceTeam::None};


    // Store position;
    int rank = 0;
    int file = 0;

    for (char c : fenString) {
        // Digit detected - Skip squares
        if (std::isdigit(c)) {
            file += (c - '0');
        }
        // Next-rank symbol detected - Skip to next rank (And reset file to 0)
        else if (c == '/') {
            rank++;
            file = 0;
        }
        // End of FEN string 
        // Note: FEN strings contain extra information after the space, but we don't need it for placing pieces.
        else if (c == ' ') {
            break;
        }
        // Piece detected - Add it's pieceInfo to the board array.
        else {
            PieceInfo pieceInfo = pieceCodes[c];
            boardState[file][rank] = pieceInfo;
            // Move to next square
            file++;
        }
    }
    return boardState;
}