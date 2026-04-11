// Defines information about the geometry painter.

#pragma once
// Standard Library
#include <string_view>
#include <filesystem>

namespace board {

    inline std::filesystem::path piecesPath{"assets/image/themes/Cburnett/"};

    namespace defaults {

        constexpr std::string_view defaultBoardFEN{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};

    }
}