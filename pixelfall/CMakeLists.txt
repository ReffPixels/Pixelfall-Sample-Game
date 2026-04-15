cmake_minimum_required(VERSION 4.3)
set(CMAKE_CXX_STANDARD 17)

# Sources are stored in sources.txt to keep CMakeLists clean.
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/sources.txt" SOURCES)
add_library(pixelfall STATIC ${SOURCES})

# Define directories used in includes
target_include_directories(pixelfall PUBLIC 
    # Use parent folder so includes are formatted as include "pixelfall/{...}"
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# OpenGL with GLAD (This is currently the only graphics library so it's used in all supported platforms)
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include)
target_link_libraries(pixelfall PRIVATE glad)

# External libraries built from source (SDL3 and SDL_ttf)
include(FetchContent)

# Register SDL3
FetchContent_Declare(SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.4.4
    GIT_SHALLOW    TRUE
)

# Register SDL_ttf
set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE) # Vendor SDL_ttf's dependencies
FetchContent_Declare(SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG        release-3.2.2
    GIT_SHALLOW    TRUE
)

# Build External Libraries
FetchContent_MakeAvailable(SDL3 SDL3_ttf)
target_link_libraries(pixelfall PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)