// Defines information about Chess and reusable types.

#pragma once
// Engine
#include "pixelfall/engine/math/vector2.h"

enum class PieceType {
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn,
    None
};

enum class TeamColor {
    White,
    Black,
    None
};

struct Piece {
    PieceType type;
    TeamColor team;
    Vector2Int position;
};

enum class MoveType {
    Move,               // A move into an empty square.
    Capture,            // A move that takes an enemy piece.
    EnPassant,          // Special pawn move.
    CastlingKingSide,   // King initiates Castling on the king's side of the board (Rook moves two squares).
    CastlingQueenSide,  // King initiates Castling on the queen's side of the board (Rook moves three squares).
    Promotion,          // When a pawn moves to the last rank it must promote to a Queen, Rook, Knight or Bishop.
    CapturePromotion,   // A promotion triggered by a pawn capturing a piece instead of moving forwards.
    None
};

struct Move {
    Vector2Int origin;
    Vector2Int target;
    MoveType type;
};

struct CastlingRights {
    bool whiteKingSide;     // White is still allowed to castle on the king's side.
    bool whiteQueenSide;    // White is still allowed to castle on the queen's side.
    bool blackKingSide;     // Black is still allowed to castle on the king's side.
    bool blackQueenSide;    // Black is still allowed to castle on the queen's side.
};

enum class GameOutcome {
    Playing,                            // The game is currently in progress
    WhiteVictoryCheckmate,              // There are no legal moves for black and the king is under check
    BlackVictoryCheckmate,              // There are no legal moves for white and the king is under check
    WhiteVictoryResignation,            // [TODO LONG TERM] Button system
    BlackVictoryResignation,            // [TODO LONG TERM] Button system
    WhiteVictoryTimeout,                // [TODO LONG TERM] Clock
    BlackVictoryTimeout,                // [TODO LONG TERM] Clock
    DrawTimeoutVsInsufficientMaterial,  // [TODO LONG TERM] Clock
    DrawStalemate,                      // There are no legal moves for the player but their king is not under check
    DrawInsufficientMaterial,           // Checkmate is impossible (The pieces left on the board are insufficient)
    Draw50Move,                         // [TODO LONG TERM] Button system (50 Move is not automatic)
    Draw75Move,                         // 75 Half moves have passed since the last capture or pawn move.
    Draw3FoldRepetition,                // [TODO LONG TERM] Fen snapshots
    Draw5FoldRepetition,                // [TODO LONG TERM] Fen snapshots
    DrawAgreement,                      // [TODO LONG TERM] Button system
};

// Default initial state of the board encoded in FEN
constexpr std::string_view defaultBoardFEN{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};