// [TODO] describe

#pragma once

#include "chess_state.h"

class ChessMoves {
public:
    // Piece Movement
    static std::array<std::array<MoveType, 8>, 8> generateKingMoves(
        Vector2Int moveOrigin, PieceTeam pieceTeam, std::array<std::array<PieceInfo, 8>, 8> boardState,
        CastlingRights& castlingRights);
    static std::array<std::array<MoveType, 8>, 8> generateQueenMoves(
        Vector2Int moveOrigin, PieceTeam pieceTeam, std::array<std::array<PieceInfo, 8>, 8> boardState);
    static std::array<std::array<MoveType, 8>, 8> generateRookMoves(
        Vector2Int moveOrigin, PieceTeam pieceTeam, std::array<std::array<PieceInfo, 8>, 8> boardState);
    static std::array<std::array<MoveType, 8>, 8> generateBishopMoves(
        Vector2Int moveOrigin, PieceTeam pieceTeam, std::array<std::array<PieceInfo, 8>, 8> boardState);
    static std::array<std::array<MoveType, 8>, 8> generateKnightMoves(
        Vector2Int moveOrigin, PieceTeam pieceTeam, std::array<std::array<PieceInfo, 8>, 8> boardState);
    static std::array<std::array<MoveType, 8>, 8> generatePawnMoves(
        Vector2Int moveOrigin, PieceTeam pieceTeam, std::array<std::array<PieceInfo, 8>, 8> boardState,
        Vector2Int enPassantTargetSquare, bool isFirstMove = false);

    // Helpers
    static std::array<std::array<MoveType, 8>, 8> ChessMoves::generateMoves(
        Vector2Int moveOrigin, PieceTeam pieceTeam,
        std::array<std::array<PieceInfo, 8>, 8> boardState,
        std::vector<Vector2Int> directions, int maxSteps);
    static void clearMoves(std::array<std::array<MoveType, 8>, 8>& moves);
};