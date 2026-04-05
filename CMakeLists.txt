cmake_minimum_required(VERSION 4.3)
project(sampleGame)
set(CMAKE_CXX_STANDARD 17)

# Sources are stored in sources.txt to keep CMakeLists clean.
file(STRINGS "${CMAKE_SOURCE_DIR}/sources.txt" SOURCES)
add_executable(sampleGame ${SOURCES})

target_include_directories(sampleGame PRIVATE 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

# OpenGL with GLAD (Currently used in every platform)
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)
target_link_libraries(sampleGame PRIVATE glad)

if(WIN32)
    target_include_directories(
        sampleGame PRIVATE ${CMAKE_SOURCE_DIR}/external/sdl/include
    )
    target_link_libraries(sampleGame PRIVATE
        ${CMAKE_SOURCE_DIR}/external/sdl/lib/SDL3.lib
        ${CMAKE_SOURCE_DIR}/external/sdl/lib/SDL3_ttf.lib
    )

    # Copy .dll files into the build folder.
    add_custom_command(TARGET sampleGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/external/sdl/bin/SDL3.dll $<TARGET_FILE_DIR:sampleGame>
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/external/sdl/bin/SDL3_ttf.dll $<TARGET_FILE_DIR:sampleGame>
    # Copy necessary engine files into the build folder.
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/pixelfall/assets $<TARGET_FILE_DIR:sampleGame>/pixelfall/assets
    )
else()
    find_package(SDL3 REQUIRED)
    find_package(SDL3_ttf REQUIRED)
    target_link_libraries(sampleGame PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)
    
    # Copy necessary engine files into the build folder.
    add_custom_command(TARGET sampleGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/pixelfall/assets $<TARGET_FILE_DIR:sampleGame>/pixelfall/assets
    )
endif()