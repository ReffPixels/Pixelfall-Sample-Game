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

enum InputState {
    Normal,
    PieceSelected,
};

class MyGame : public Application {
public:
    bool onStart()  override;
    void onUpdate() override;
    void onRender() override;

    void movePiece(PieceInfo& piece, std::string newPos);
    void nextTurn();
private:
    std::string currentBoardFEN = board::defaults::defaultBoardFEN.data();
    FenParser fenParser;
    ChessPieces pieces;
    std::vector<PieceInfo> piecesInfo;
    std::unique_ptr<ChessBoard> board;

    // Position in piecesInfo vector of the selected piece.
    int selectedPieceInfo{};
    InputState inputState{Normal};
    PieceTeam playerTeam{};
};