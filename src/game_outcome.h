// Tools to detect the outcome of a chess game

#pragma once
// Chess
#include "config/chess_config.h"
#include "game_state.h"
#include "game_objects/chess_board.h"
#include "game_objects/chess_pieces.h"
// Standard Library
#include <memory>
#include <array>
#include <vector>

namespace game_outcome {
    
    Outcome getOutcome(const BoardState& boardState);

    // Methods used to determine game outcome
    bool isKingInCheck(const BoardState& boardState);
    bool hasLegalMoves(const BoardState& boardState);
    bool hasInsufficientMaterial(const BoardState& boardState);
    TileColor getTileColor(const Vector2Int& position);

    // Other
    void displayOutcome(Outcome outcome);
};