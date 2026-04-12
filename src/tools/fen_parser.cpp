// Implementation for fen_parser.h

#include "tools/fen_parser.h"
#include "tools/notation_parser.h"
#include <sstream>

// Parse the section of a FEN string that encodes the list of pieces
// Returns false if the string is invalid.
static bool parsePieceList(const std::string& pieceList, FenState& fenState) {
    // Store position (X = File and Y = Rank)
    Vector2Int gridPosition{0, 0};

    for (char c : pieceList) {
        // Digit detected - Skip squares
        if (std::isdigit(c)) {
            gridPosition.x += (c - '0');
        }
        // Next-rank symbol detected - Skip to next rank (And reset file to 0)
        else if (c == '/') {
            gridPosition.y++;
            gridPosition.x = 0;
        }
        // Piece detected - Add it to the FEN state
        else if (std::string("KQRBNPkqrbnp").find(c) != std::string::npos) {
            fenState.pieceList.push_back(fenParser::getPieceFromSymbol(c, gridPosition));
            // Move to next square
            gridPosition.x++;
        }
        // Invalid character
        else return false;
    }
    return true;
}

// Parse the section of a FEN string that encodes the active player
// Returns false if the string is invalid.
static bool parsePlayerToMove(const std::string& playerToMove, FenState& fenState) {
    // Player to move should be exactly one character
    if (playerToMove.size() != 1) return false;

    if (playerToMove[0] == 'w') {
        fenState.playerToMove = TeamColor::White;
        return true;
    }
    else if (playerToMove[0] == 'b') {
        fenState.playerToMove = TeamColor::Black;
        return true;
    }
    // Invalid character
    else return false;
}

// Parse the section of a FEN string that encodes castling rights (KQkq)
// Returns false if the string is invalid.
static bool parseCastlingRights(const std::string& castlingRights, FenState& fenState) {

    for (char c : castlingRights) {
        if (c == 'K') {
            fenState.castlingRights.whiteKingSide = true;
        }
        else if (c == 'Q') {
            fenState.castlingRights.whiteQueenSide = true;
        }
        else if (c == 'k') {
            fenState.castlingRights.blackKingSide = true;
        }
        else if (c == 'q') {
            fenState.castlingRights.blackQueenSide = true;
        }
        else if (c == '-') {
            // Do nothing, castling rights are false by default
            return true;
        }
        // Invalid Character
        else return false;
    }
    return true;
}

// Parse the section of a FEN string that encodes the en passant target square.
// Returns false if the string is invalid.
static bool parseEnPassantSquare(const std::string& enPassantTargetSquare, FenState& fenState) {
    // No en passant square (Not an error, just skip assignment and leave the default)
    if (enPassantTargetSquare == "-") return true;

    // Square in algebraic notation should be exactly 2 characters
    if (enPassantTargetSquare.size() != 2) return false;

    // Check that the notation is in the correct format
    if (std::string("abcdefgh").find(enPassantTargetSquare[0]) != std::string::npos
        && std::string("12345678").find(enPassantTargetSquare[1]) != std::string::npos) {
        fenState.enPassantTargetSquare = NotationParser::getPosFromNotation(enPassantTargetSquare);
        return true;
    }

    // Invalid format
    return false;
}


// Parse the section of a FEN string that encodes half moves (50/75 move rule). 
// Returns false if the string is invalid.
static bool parseMoveRuleCounter(const std::string & moveRuleCounter, FenState & fenState) {
    // Store half moves
    int halfMoves{0};

    for (char c : moveRuleCounter) {
        if (std::isdigit(c)) {
            // New digit detected. Multiply previous number by 10 since this digit is the next position in base 10
            halfMoves = (halfMoves * 10) + (c - '0');
        }
        // Invalid Character
        else return false;
    }
    fenState.moveRuleCounter = halfMoves;
    return true;
}

// Parse the section of a FEN string that encodes total full moves. 
// Returns false if the string is invalid.
static bool parseTotalFullMoves(const std::string & totalFullMoves, FenState & fenState) {
    // Store full moves
    int totalMoves{0};

    for (char c : totalFullMoves) {
        if (std::isdigit(c)) {
            // New digit detected. Multiply previous number by 10 since this digit is the next position in base 10
            totalMoves = (totalMoves * 10) + (c - '0');
        }
        // Invalid Character
        else return false;
    }
    fenState.totalFullMoves = totalMoves;
    return true;
}

// Transforms a piece in symbol format (Like k for black king) into a usable Piece struct.
// Position is required for a piece struct.
Piece fenParser::getPieceFromSymbol(char symbol, Vector2Int position) {
    switch (symbol) {
        // White Pieces
    case 'K': return Piece{PieceType::King, TeamColor::White, position};
    case 'Q': return Piece{PieceType::Queen, TeamColor::White, position};
    case 'R': return Piece{PieceType::Rook, TeamColor::White, position};
    case 'B': return Piece{PieceType::Bishop, TeamColor::White, position};
    case 'N': return Piece{PieceType::Knight, TeamColor::White, position};
    case 'P': return Piece{PieceType::Pawn, TeamColor::White, position};
            // Black Pieces
    case 'k': return Piece{PieceType::King, TeamColor::Black, position};
    case 'q': return Piece{PieceType::Queen, TeamColor::Black, position};
    case 'r': return Piece{PieceType::Rook, TeamColor::Black, position};
    case 'b': return Piece{PieceType::Bishop, TeamColor::Black, position};
    case 'n': return Piece{PieceType::Knight, TeamColor::Black, position};
    case 'p': return Piece{PieceType::Pawn, TeamColor::Black, position};
            // Invalid Symbol
    default: return Piece{PieceType::None, TeamColor::None, position};
    }
}

// Used to split a string by a certain character
static std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream stream(str);
    while (std::getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Parse an entire fen string and return its values in a usable struct format.
// Returns a default empty board struct if the format is invalid.
FenState fenParser::getState(const std::string& fenString) {

    // Split the FEN string into its different categories
    std::vector<std::string> fenCategories = split(fenString, ' ');

    // Create FEN State
    FenState fenState;

    // Make sure the FEN string is complete
    if (fenCategories.size() != 6) return FenState{}; // If this fenState is invalid, return the default (Empty board)

    // Decode every category and check that none of them returned false (Invalid string format)
    if (!parsePieceList(fenCategories[0], fenState)
        || !parsePlayerToMove(fenCategories[1], fenState)
        || !parseCastlingRights(fenCategories[2], fenState)
        || !parseEnPassantSquare(fenCategories[3], fenState)
        || !parseMoveRuleCounter(fenCategories[4], fenState)
        || !parseTotalFullMoves(fenCategories[5], fenState)
        ) return FenState{};

    return fenState;
}