// [TODO] describe

#pragma once

#include "config/chess_config.h"
#include <array>
#include <vector>

namespace move_generation {
    // Piece Movement
     void findLegalMovesForPiece(std::array<std::array<MoveType, 8>, 8>& moves,
        Tile piece, Vector2Int position, const std::array<std::array<Tile, 8>, 8>& boardState,
        const CastlingRights& castlingRights, Vector2Int enPassantTargetSquare = Vector2Int(-1, -1));

     std::array<std::array<MoveType, 8>, 8> generateMovesForPiece(
        Tile piece, Vector2Int position, const std::array<std::array<Tile, 8>, 8>& boardState,
         const CastlingRights& castlingRights, Vector2Int enPassantTargetSquare = Vector2Int(-1, -1), bool attackMode = false);

     std::array<std::array<MoveType, 8>, 8> generateMoves(
        Vector2Int moveOrigin, TeamColor TeamColor,
        const std::array<std::array<Tile, 8>, 8>& boardState,
         std::vector<Vector2Int> directions, int maxSteps, bool attackMode = false);

     std::array<std::array<bool, 8>, 8> getAttackedSquares(bool ignoreKing, TeamColor playerTeam,
        const std::array<std::array<Tile, 8>, 8>& boardState, const CastlingRights& castlingRights);

     std::array<std::array<MoveType, 8>, 8> generateKingMoves(
        Vector2Int moveOrigin, TeamColor TeamColor, const std::array<std::array<Tile, 8>, 8>& boardState,
         const CastlingRights& castlingRights, bool attackMode = false);
    
     std::array<std::array<MoveType, 8>, 8> generateQueenMoves(
         Vector2Int moveOrigin, TeamColor TeamColor, const std::array<std::array<Tile, 8>, 8>& boardState, bool attackMode = false);
    
     std::array<std::array<MoveType, 8>, 8> generateRookMoves(
         Vector2Int moveOrigin, TeamColor TeamColor, const std::array<std::array<Tile, 8>, 8>& boardState, bool attackMode = false);
    
     std::array<std::array<MoveType, 8>, 8> generateBishopMoves(
         Vector2Int moveOrigin, TeamColor TeamColor, const std::array<std::array<Tile, 8>, 8>& boardState, bool attackMode = false);
    
     std::array<std::array<MoveType, 8>, 8> generateKnightMoves(
         Vector2Int moveOrigin, TeamColor TeamColor, const std::array<std::array<Tile, 8>, 8>& boardState, bool attackMode = false);
    
     std::array<std::array<MoveType, 8>, 8> generatePawnMoves(
        Vector2Int moveOrigin, TeamColor TeamColor, const std::array<std::array<Tile, 8>, 8>& boardState,
         Vector2Int enPassantTargetSquare, bool isFirstMove = false, bool attackMode = false);

     void clearMoves(std::array<std::array<MoveType, 8>, 8>& moves);
};