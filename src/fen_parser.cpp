// Implementation for fen_parser.h

#include "fen_parser.h"

// Populates pieces vector with the correct placement information taken from a FEN string.
std::vector<PieceInfo> FenParser::getPiecesFromFEN(const std::string fenString) {
    // Reset pieces container
    std::vector<PieceInfo> pieces;

    // Store position as a simple integer (0 to 63) - Will be transformed into a Vector2{rank, file}
    int pieceRank = 0;
    int pieceFile = 0;

    for (char c : fenString) {
        // Digit detected - Skip squares
        if (std::isdigit(c)) {
            pieceFile += (c - '0');
        }
        // Next-rank symbol detected - Skip to next rank (And reset file to 0)
        else if (c == '/') {
            pieceRank++;
            pieceFile = 0;
        }
        // End of FEN string 
        // Note: FEN strings contain extra information after the space, but we don't need it for placing pieces.
        else if (c == ' ') {
            break;
        }
        // Piece detected - Add it's pieceInfo to the pieces vector.
        else {
            PieceInfo pieceInfo = pieceCodes[c];
            pieceInfo.position = {pieceFile, pieceRank};
            pieces.push_back(pieceInfo);
            // Move to next square
            pieceFile++;
        }
    }
    return pieces;
}