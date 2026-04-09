// [TODO] describe

#pragma once

#include "chess_state.h"

class ChessMoves {
public:
    static std::array<std::array<MoveType, 8>, 8> GenerateRookMoves(
        Vector2Int moveOrigin, PieceTeam rookTeam, std::array<std::array<PieceInfo, 8>, 8> boardState);
};