// Defines information about the geometry painter.

#pragma once
// Standard Library
#include <string_view>
#include "chess_theme.h"

namespace board {

    namespace defaults {

        constexpr std::string_view defaultBoardFEN{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};

        constexpr bool isometricProjection{false};
        constexpr float isometricPieceScale{0.75};
    }
}