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

if(WIN32)
    # Using SDL library
    add_library(SDL3 SHARED IMPORTED GLOBAL)
    set_target_properties(SDL3 PROPERTIES
        IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl/bin/SDL3.dll
        IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl/lib/SDL3.lib
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl/include
    )

    # Using SDL_ttf library
    add_library(SDL3_ttf SHARED IMPORTED GLOBAL)
    set_target_properties(SDL3_ttf PROPERTIES
        IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl/bin/SDL3_ttf.dll
        IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl/lib/SDL3_ttf.lib
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl/include
    )

    # Link Libraries
    target_link_libraries(pixelfall PRIVATE SDL3 SDL3_ttf)
else()
    # Using SDL library
    find_package(SDL3 REQUIRED)

    # Using SDL_ttf library
    find_package(SDL3_ttf REQUIRED)

    # Link Libraries
    target_link_libraries(pixelfall PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)
endif()