// Defines the loop of this specific application.

#include "chess.h"
// Standard Library
#include <iostream>
#include <algorithm>

// Create Game
PIXELFALL_APPLICATION(Chess);

// Called on the first frame
bool Chess::onStart() {
    // Set Metadata
    appWindow->setWindowTitle("Chess");
    appTitle = "Chess";
    appVersion = "0.1";
    appIdentifier = "chess";

    // Window Settings
    appWindow->setWReferenceSize({720, 720});

    // Start new chess game
    startNewGame();

    return true;
}

// Called every frame
void Chess::onUpdate() {

    // Track FPS in window title
    appWindow->setWindowTitle("Chess | FPS: " + std::to_string(appClock->getFPS()));

    // Store Cursor Position
    cursorPos = appWindow->physicalToLogical(appInput->getMousePhysicalPosition());

    // Center Board
    board.setPosition((appWindow->getLogicalSize() / 2) - (board.getTileSize() * 4));

    // Handle board interaction
    if (state.getGameOutcome() == GameOutcome::Playing) {
        if (board.isBoardOnHover(cursorPos)) {
            Vector2Int hoveredSquare = board.getTileOnHover(cursorPos);
            if (appInput->isMouseButtonPressed(MouseButton::Left)) onBoardPressed(hoveredSquare);
            // [TEMP DEBUG] Delete Piece on Hover
            if (appInput->isKeyPressed(KeyCode::Delete)) {
                state.removePiece(hoveredSquare);
                nextTurn();
            }
            if (appInput->isMouseButtonReleased(MouseButton::Left)) onBoardReleased(hoveredSquare);
        }
        // Promotion UI Interaction
        if (inputState == InputState::Promotion) {
            if (appInput->isMouseButtonPressed(MouseButton::Left)) {
                bool flipPieces = board.getBoardDirection() != BoardDirection::BlackOnTop;
                PieceType chosenPiece = PromotionInterface::getPieceOnHover(cursorPos, state.getPromotionPosition(),
                    state.getBoardState().playerToMove, board.getPosition(), board.getTileSize(), flipPieces);
                if (chosenPiece != PieceType::None)
                    state.onPromotionSelected(chosenPiece);
            }
        }

    }
    // Game has ended, only action is to reset the game
    else if (appInput->isMouseButtonPressed(MouseButton::Left)) startNewGame();
}

// Called at the end of each frame (Handles rendering)
void Chess::onRender() {
    BoardState boardState = state.getBoardState();

    // Draw Background
    Color backgroundColor{
        state.getGameOutcome() == GameOutcome::Playing ?
        Color::fromHexcode("#212121") : Color::fromHexcode("#33ff00")};
    
    painter->drawRectangle(
        Vector2::Zero,
        Vector2(appWindow->getLogicalSize().x, appWindow->getLogicalSize().y),
        backgroundColor
    );

    // Draw Board
    board.draw(*painter);

    // Draw Attacked Squares (Player)
    if (appInput->isKeyDown(KeyCode::Q)) TileHighlights::highlightAttackedSquares(MoveGeneration::getAttackedSquares(
        false, boardState.playerToMove, boardState.tiles, boardState.castlingRights), board, *painter);

    // Draw Attacked Squares (Opponent)
    if (appInput->isKeyDown(KeyCode::E)) TileHighlights::highlightAttackedSquares(MoveGeneration::getAttackedSquares(
        false, state.getOpponent(), boardState.tiles, boardState.castlingRights), board, *painter,
        Color::fromHexcode("#0000ff88"));

    // Draw Highlights
    Move lastMove = state.getLastMove();
    TileHighlights::highlightLastMove(board, lastMove.origin, lastMove.target, *painter);

    if (inputState == InputState::PieceSelected) {
        TileHighlights::highlightSelected(board, selPiecePosition, *painter);
        TileHighlights::highlightValidMoves(state.getValidMoves(), board, *painter);
    }

    bool dragAndDrop = inputState == InputState::PieceSelected
        && appInput->isMouseButtonDown(MouseButton::Left);
    // Draw Drag and Drop Highlight (Under pieces)
    if (dragAndDrop) TileHighlights::highlightHoveredSquare(cursorPos, board, *painter, selPiecePosition);

    // Draw Pieces (hide selected piece during drag to avoid duplication)
    // [TODO] remove selected piece from list somehow
    pieces.drawPieces(boardState.pieces, board, *painter);

    // Drag and Drop Visuals (piece following cursor, over pieces)
    // [TODO] get selected piece
    if (dragAndDrop) {
        pieces.pieceFollowCursor(cursorPos, board, PieceType::King, TeamColor::White, *painter, dragAndDropPivot);
    }

    // Draw Promotion UI
    if (inputState == InputState::Promotion) {
        PromotionInterface::drawPieces(pieces, state.getPromotionPosition(), boardState.playerToMove,
            board.getPosition(), board.getTileSize(), board.getTileSize() * 0.8f, *painter,
            board.getBoardDirection() == BoardDirection::BlackOnTop ? false : true);
    }
}

void Chess::startNewGame() {
    // Randomly choose the player team using Bernoulli Distribution 
     // This returns a bool (True or False) at a given chance (50 % in this case) by using our random sequence (rng)
    std::bernoulli_distribution coinFlip(0.5);
    TeamColor playerTeam = (coinFlip(rng)) ? TeamColor::White : TeamColor::Black;

    // Set initial state of board.
    state.resetGame();

    // We need to flip the board and pieces here since state does not handle board TileHighlights::
    if (playerTeam == TeamColor::White) {
        board.setBoardDirection(BoardDirection::BlackOnTop);
        pieces.setFlippedPieces(false);
    }
    else {
        board.setBoardDirection(BoardDirection::WhiteOnTop);
        pieces.setFlippedPieces(true);
    }
}

// Swaps the current player and runs necessary functions to set up the next turn (Or end the game)
void Chess::nextTurn() {
    // Swap player
    playerToMove = (playerToMove == TeamColor::White ? TeamColor::Black : TeamColor::White);
    // Count full moves
    if (playerToMove == TeamColor::Black) totalFullMoves++;

    deselectPiece();
    updateCastlingRights();
    updatePieceList();
    printStatistics();
    findGameOutcome();
}

void Chess::endGame() {
    std::cout << "-------------------------------" << std::endl;
    switch (gameOutcome) {
    case GameOutcome::WhiteVictoryCheckmate: std::cout << "WHITE VICTORY (CHECKMATE)" << std::endl; break;
    case GameOutcome::BlackVictoryCheckmate: std::cout << "BLACK VICTORY (CHECKMATE)" << std::endl; break;
    case GameOutcome::WhiteVictoryResignation: std::cout << "WHITE VICTORY (RESIGNATION)" << std::endl; break;
    case GameOutcome::BlackVictoryResignation: std::cout << "BLACK VICTORY (RESIGNATION)" << std::endl; break;
    case GameOutcome::WhiteVictoryTimeout: std::cout << "WHITE VICTORY (TIMEOUT)" << std::endl; break;
    case GameOutcome::BlackVictoryTimeout: std::cout << "BLACK VICTORY (TIMEOUT)" << std::endl; break;
    case GameOutcome::DrawStalemate: std::cout << "DRAW (STALEMATE)" << std::endl; break;
    case GameOutcome::DrawInsufficientMaterial: std::cout << "DRAW (INSUFFICIENT MATERIAL)" << std::endl; break;
    case GameOutcome::Draw50Move: std::cout << "DRAW (50 MOVE RULE)" << std::endl; break;
    case GameOutcome::Draw75Move: std::cout << "DRAW (75 MOVE RULE)" << std::endl; break;
    case GameOutcome::Draw3FoldRepetition: std::cout << "DRAW (3 FOLD REPETITION)" << std::endl; break;
    case GameOutcome::Draw5FoldRepetition: std::cout << "DRAW (5 FOLD REPETITION)" << std::endl; break;
    case GameOutcome::DrawAgreement: std::cout << "DRAW (AGREEMENT)" << std::endl; break;
    }
}

void Chess::resetGame() {
    // White always goes first, even if we are playing from black's perspective
    playerToMove = TeamColor::White;

    // Reset board state and trackers
    gameOutcome = GameOutcome::Playing;
    castlingRights = {true, true, true, true};
    enPassantTargetSquare = {-1, -1};
    moveRuleCounter = 0;
    totalFullMoves = 1;
    lastMoveOrigin = {-1, -1};
    lastMoveTarget = {-1, -1};
    setupFromFEN();

    printStatistics();
}

void Chess::printStatistics() {
    std::cout << "-------------------------------" << '\n';
    std::cout << "Player to Move: " << static_cast<int>(playerToMove) << '\n';
    std::cout << "Half Moves: " << moveRuleCounter << '\n';
    std::cout << "Full Moves: " << totalFullMoves << '\n';
    std::cout << "Castling Rights KQkq: " << castlingRights.whiteKingSide << castlingRights.whiteQueenSide
        << castlingRights.blackKingSide << castlingRights.blackQueenSide << std::endl;
}

// Pressing allows to select a new piece or move if a piece is already selected.
void Chess::onBoardPressed(Vector2Int square) {
    // Capture drag pivot at the moment of selection
    if (inputState == InputState::PieceSelected)
        dragAndDropPivot = ChessPieces::computeDragPivot(cursorPos, board);
    
    PieceInfo& clicked = boardState[square.x][square.y];
    if (inputState == InputState::Promotion) return;

    // It's a piece in the player's team
    if (clicked.type != PieceType::None && clicked.team == playerToMove)
        // Clicked on the same square, deselect
        if (square == selPiecePosition) {
            deselectPiece();
            return;
        }
        else selectPiece(square);
    else
        moveSelectedPiece(square);
}

// Release only allows to move if a piece is already selected. (Drag and drop behaviour) Otherwise it does nothing.
void Chess::onBoardReleased(Vector2Int square) {
    if (inputState == InputState::Promotion) return;

    moveSelectedPiece(square);
}

// Selects a piece in a specific square and changes the input state to selected.
void Chess::selectPiece(Vector2Int selectedSquare) {
    selPiecePosition = selectedSquare;
    PieceInfo& selPieceInfo = boardState[selPiecePosition.x][selPiecePosition.y];
    inputState = InputState::PieceSelected;
    validMoves = ChessMoves::generateMovesForPiece(
        selPieceInfo, selPiecePosition, boardState, castlingRights, enPassantTargetSquare);
    ChessMoves::findLegalMovesForPiece(
        validMoves, selPieceInfo, selPiecePosition, boardState, castlingRights, enPassantTargetSquare);
}

// Reset selected piece trackers
void Chess::deselectPiece() {
    selPiecePosition = {-1, -1};
    inputState = InputState::Normal;
    ChessMoves::clearMoves(validMoves);
}

// Moves the selected piece to a new square and updates the board state to match. 
// A succesful move triggers the next turn.
void Chess::moveSelectedPiece(Vector2Int targetSquare) {
    MoveType moveType = validMoves[targetSquare.x][targetSquare.y];

    if (inputState == InputState::Normal) return;
    if (targetSquare == selPiecePosition) return;
    if (moveType == MoveType::None) {
        deselectPiece();
        return;
    }

    movePiece(selPiecePosition, targetSquare, moveType);

    if (inputState != InputState::Promotion) {
        nextTurn();
    }
}
