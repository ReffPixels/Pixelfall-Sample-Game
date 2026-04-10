// [TODO] describe

#pragma once

#include "chess_state.h"

class ChessMoves {
public:
    // Piece Movement
    static void findLegalMovesForPiece(std::array<std::array<MoveType, 8>, 8>& moves,
        PieceInfo piece, Vector2Int position, const std::array<std::array<PieceInfo, 8>, 8>& boardState,
        const CastlingRights& castlingRights, Vector2Int enPassantTargetSquare = Vector2Int(-1, -1));

    static std::array<std::array<MoveType, 8>, 8> generateMovesForPiece(
        PieceInfo piece, Vector2Int position, const std::array<std::array<PieceInfo, 8>, 8>& boardState,
        const CastlingRights& castlingRights, Vector2Int enPassantTargetSquare = Vector2Int(-1, -1));

    static std::array<std::array<MoveType, 8>, 8> ChessMoves::generateMoves(
        Vector2Int moveOrigin, PieceTeam pieceTeam,
        const std::array<std::array<PieceInfo, 8>, 8>& boardState,
        std::vector<Vector2Int> directions, int maxSteps);

    static std::array<std::array<MoveType, 8>, 8> generateKingMoves(
        Vector2Int moveOrigin, PieceTeam pieceTeam, const std::array<std::array<PieceInfo, 8>, 8>& boardState,
        const CastlingRights& castlingRights);
    
    static std::array<std::array<MoveType, 8>, 8> generateQueenMoves(
        Vector2Int moveOrigin, PieceTeam pieceTeam, const std::array<std::array<PieceInfo, 8>, 8>& boardState);
    
    static std::array<std::array<MoveType, 8>, 8> generateRookMoves(
        Vector2Int moveOrigin, PieceTeam pieceTeam, const std::array<std::array<PieceInfo, 8>, 8>& boardState);
    
    static std::array<std::array<MoveType, 8>, 8> generateBishopMoves(
        Vector2Int moveOrigin, PieceTeam pieceTeam, const std::array<std::array<PieceInfo, 8>, 8>& boardState);
    
    static std::array<std::array<MoveType, 8>, 8> generateKnightMoves(
        Vector2Int moveOrigin, PieceTeam pieceTeam, const std::array<std::array<PieceInfo, 8>, 8>& boardState);
    
    static std::array<std::array<MoveType, 8>, 8> generatePawnMoves(
        Vector2Int moveOrigin, PieceTeam pieceTeam, const std::array<std::array<PieceInfo, 8>, 8>& boardState,
        Vector2Int enPassantTargetSquare, bool isFirstMove = false);

    static void clearMoves(std::array<std::array<MoveType, 8>, 8>& moves);
};