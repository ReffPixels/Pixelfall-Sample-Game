// Defines the loop of this specific application. 
// Overwrites the main functions defined in application.h (Run by the engine).

#pragma once
// Engine
#include "pixelfall/engine/core/application.h"
// Chess
#include "config/board_config.h"
#include "chess_pieces.h"
#include "chess_board.h"
#include "fen_parser.h"
// Standard Library
#include <memory>
#include <array>

enum InputState {
    Normal,
    PieceSelected,
};

class MyGame : public Application {
public:
    bool onStart()  override;
    void onUpdate() override;
    void onRender() override;

    // Chess Methods
    void onBoardPressed(Vector2Int square);
    void onBoardReleased(Vector2Int square);
    void selectPiece(Vector2Int square);
    void moveSelectedPiece(Vector2Int square);
    void nextTurn();

private:
    Vector2 mousePos{};
    
    std::string currentBoardFEN = board::defaults::defaultBoardFEN.data();
    FenParser fenParser;
    ChessPieces pieces;
    std::unique_ptr<ChessBoard> board;

    // Position in piecesInfo vector of the selected piece.
    //std::vector<PieceInfo> piecesInfo;
    //int selectedPieceInfo{-1};

    // Input
    InputState inputState{Normal};
    PieceTeam playerTeam{};

    // Board State
    // 8x8 2D array of files and ranks 
    std::array<std::array<PieceInfo, 8>, 8> boardState;
    Vector2Int selectedPiecePosition{-1, -1};  // (-1, -1) means no square is selected.
};