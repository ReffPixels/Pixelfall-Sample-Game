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
    if (gameOutcome == Outcome::Playing) {
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
                PieceType chosenPiece = promotion_interface::getPieceOnHover(cursorPos, state.getLastMove().target,
                    state.getBoardState().playerToMove, board.getPosition(), board.getTileSize(), flipPieces);
                if (chosenPiece != PieceType::None) {
                    state.selectPromotionPiece(chosenPiece);
                    nextTurn();
                }
            }
        }

    }
    // Game has ended, only action is to reset the game
    else if (appInput->isMouseButtonPressed(MouseButton::Left)) startNewGame();


    // Advance Piece Movement Animation
    if (pieceAnimation.isActive())
        pieceAnimation.progress = std::min(1.0f,
            pieceAnimation.progress + (float)(appClock->getDeltaTime() / PieceAnimation::duration));
}

// Called at the end of each frame (Handles rendering)
void Chess::onRender() {
    BoardState boardState = state.getBoardState();

    // Draw Background
    Color backgroundColor{
        gameOutcome == Outcome::Playing ?
        Color::fromHexcode("#1b1b1d") : Color::fromHexcode("#33ff00")};
    
    painter->drawRectangle(
        Vector2::Zero,
        Vector2(appWindow->getLogicalSize().x, appWindow->getLogicalSize().y),
        backgroundColor
    );

    // Draw board
    float oSize{10.0f};
    float bCornerRadius{board.getTileSize().x * 0.25f};
    float oCornerRadius{board.getTileSize().x * 0.35f};

    // Board Outline
    painter->drawRectangleRound(
        board.getPosition() - Vector2(oSize, oSize),
        (board.getTileSize() * 8.0f) + Vector2(oSize, oSize) * 2,
        Color::fromHexcode("#35373a"),
        oCornerRadius
    );

    // Board Tiles
    board.drawRound(*painter, bCornerRadius);

    // Draw Attacked Squares (Player)
    if (appInput->isKeyDown(KeyCode::Q)) tile_highlights::highlightAttackedSquares(move_generation::getAttackedSquares(
        false, boardState.playerToMove, boardState.tiles, boardState.castlingRights), board, *painter);

    // Draw Attacked Squares (Opponent)
    if (appInput->isKeyDown(KeyCode::E)) tile_highlights::highlightAttackedSquares(move_generation::getAttackedSquares(
        false, state.getOpponent(), boardState.tiles, boardState.castlingRights), board, *painter,
        Color::fromHexcode("#0000ff88"));

    // Draw Highlights
    Move lastMove = state.getLastMove();
    tile_highlights::highlightLastMove(board, lastMove.origin, lastMove.target, *painter);

    if (inputState == InputState::PieceSelected) {
        tile_highlights::highlightSelected(board, selectedPiece.position, *painter);
        tile_highlights::highlightMoves(selectedPieceMoves, board, *painter);
    }

    bool dragAndDrop = inputState == InputState::PieceSelected
        && appInput->isMouseButtonDown(MouseButton::Left);
    // Draw Drag and Drop Highlight (Under pieces)
    if (dragAndDrop) tile_highlights::highlightHoveredSquare(cursorPos, board, *painter, selectedPiece.position);

    // Draw the pieces
    drawPieces(dragAndDrop);

    // Draw Promotion UI
    if (inputState == InputState::Promotion) {
        promotion_interface::drawPieces(pieces, state.getLastMove().target, boardState.playerToMove,
            board.getPosition(), board.getTileSize(), board.getTileSize() * 0.8f, *painter,
            board.getBoardDirection() == BoardDirection::BlackOnTop ? false : true);
    }
}

// Draws the pieces on the board and the selected piece depending on input type.
void Chess::drawPieces(bool dragAndDrop) {
    BoardState boardState = state.getBoardState();

    // Draw Pieces with the selected piece following the mouse (Drag and Drop)
    if (dragAndDrop) {
        // Draw the pieces without the selected piece to avoid duplication.
        std::vector<Piece> visiblePieces;
        for (Piece piece : boardState.pieces)
            if (piece.position != selectedPiece.position)
                visiblePieces.push_back(piece);
        pieces.drawPieces(visiblePieces, board, *painter);

        // Draw the selected piece following the mouse
        pieces.pieceFollowCursor(cursorPos, board, selectedPiece.type, selectedPiece.team, *painter, dragAndDropPivot);
    }
    // Draw Pieces with the selected piece animating towards the target square. (Click)
    else if (pieceAnimation.isActive()) {
        // Draw the pieces without the selected piece to avoid duplication.
        std::vector<Piece> visiblePieces;
        for (Piece piece : boardState.pieces)
            if (piece.position != pieceAnimation.piece.position)
                visiblePieces.push_back(piece);
        pieces.drawPieces(visiblePieces, board, *painter);

        // Draw the selected piece with animation
        float t = pieceAnimation.progress;
        t = t * t * (3.0f - 2.0f * t); // Smooth ease
        Vector2 fromPos = pieces.getDrawPosition(pieceAnimation.origin, board);
        Vector2 toPos = pieces.getDrawPosition(pieceAnimation.piece.position, board);
        pieces.drawFree(pieceAnimation.piece.type, pieceAnimation.piece.team,
            fromPos + (toPos - fromPos) * t, *painter);
    }
    // Draw the pieces at their final position.
    else {
        pieces.drawPieces(boardState.pieces, board, *painter);
    }
}

void Chess::startNewGame() {
    // Randomly choose the player team using Bernoulli Distribution 
     // This returns a bool (True or False) at a given chance (50 % in this case) by using our random sequence (rng)
    std::bernoulli_distribution coinFlip(0.5);
    TeamColor playerTeam = (coinFlip(rng)) ? TeamColor::White : TeamColor::Black;

    // Set initial state of board.
    resetGame();

    // We need to flip the board and pieces here since state does not handle board tile_highlights::
    if (playerTeam == TeamColor::White)
        board.setBoardDirection(BoardDirection::BlackOnTop);
    else
        board.setBoardDirection(BoardDirection::WhiteOnTop);
}

// Swaps the current player and runs necessary functions to set up the next turn (Or end the game)
void Chess::nextTurn() {

    // Update Game State
    state.updateCastlingRights();
    state.incrementTotalMoves();
    state.syncPieceState();

    // Let the other player move.
    deselectPiece();
    state.swapPlayers();

    // Analize the board to detect if the game has ended
    gameOutcome = game_outcome::getOutcome(state.getBoardState());
    game_outcome::displayOutcome(gameOutcome);
}

void Chess::resetGame() {
    selectedPiece.position = {-1, -1};
    selectedPiece.type = PieceType::None;
    selectedPiece.team = TeamColor::None;
    move_generation::clearMoves(selectedPieceMoves);
    state.clearState();
    gameOutcome = Outcome::Playing;
    state.setupFromFEN(defaults::startPositionFEN.data());
}

// Pressing allows to select a new piece or move if a piece is already selected.
void Chess::onBoardPressed(Vector2Int square) {
    if (inputState == InputState::Promotion) return;

    Tile tile = state.getBoardState().tiles[square.x][square.y];
    
    // Update drag and drop pivot when clicking
    dragAndDropPivot = ChessPieces::computeDragPivot(cursorPos, board);
    
    // It's a piece in the player's team
    if (tile.type != PieceType::None && tile.team == state.getBoardState().playerToMove)
        // Clicked on the same square, deselect
        if (square == selectedPiece.position) {
            deselectPiece();
            return;
        }
        else selectPiece(square);
    else
        moveSelectedPiece(square, false);
}

// Release only allows to move if a piece is already selected. (Drag and drop behaviour) Otherwise it does nothing.
void Chess::onBoardReleased(Vector2Int square) {
    if (inputState == InputState::Promotion) return;

    moveSelectedPiece(square, true);
}

// Selects a piece in a specific square and changes the input state to selected.
void Chess::selectPiece(Vector2Int selectedSquare) {
    BoardState boardState = state.getBoardState();
        
    selectedPiece = {
        state.getBoardState().tiles[selectedSquare.x][selectedSquare.y].type,
        state.getBoardState().tiles[selectedSquare.x][selectedSquare.y].team,
        selectedSquare};

    Tile selTile = state.getBoardState().tiles[selectedPiece.position.x][selectedPiece.position.y];
    inputState = InputState::PieceSelected;
    selectedPieceMoves = move_generation::generateMovesForPiece(
        selTile, selectedPiece.position, boardState.tiles, boardState.castlingRights, boardState.enPassantTargetSquare);
    move_generation::findLegalMovesForPiece(
        selectedPieceMoves, selTile, selectedPiece.position, boardState.tiles,
        boardState.castlingRights, boardState.enPassantTargetSquare);
}

// Reset selected piece trackers
void Chess::deselectPiece() {
    selectedPiece.position = {-1, -1};
    inputState = InputState::Normal;
    move_generation::clearMoves(selectedPieceMoves);
}

// Moves the selected piece to a new square and updates the board state to match.
// A succesful move triggers the next turn.
void Chess::moveSelectedPiece(Vector2Int targetSquare, bool isDrag) {
    MoveType moveType = selectedPieceMoves[targetSquare.x][targetSquare.y];

    // Don't move the piece if the move is invalid.
    if (inputState == InputState::Normal) return;
    if (targetSquare == selectedPiece.position) return;
    if (moveType == MoveType::None) { deselectPiece(); return; }

    // Move the piece.
    state.movePiece(selectedPiece.position, targetSquare, moveType);

    // Start piece movement animation for click moves only (Not drag-and-drop)
    if (!isDrag) {
        pieceAnimation.piece = {selectedPiece.type, selectedPiece.team, targetSquare};
        pieceAnimation.origin = selectedPiece.position;
        pieceAnimation.progress = 0.0f;
    }

    // If this was a promotion, don't go to next turn! We need extra input from the user (Choose promotion piece)
    if (moveType == MoveType::Promotion || moveType == MoveType::CapturePromotion) {
        inputState = InputState::Promotion;
        // Manually update the state of pieces. Normally this is handled by nextTurn() but we are skipping it.
        state.syncPieceState();
    }
    else nextTurn();
}
