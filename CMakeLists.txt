cmake_minimum_required(VERSION 4.3)
project(pixelfall)
set(CMAKE_CXX_STANDARD 17)

# Sources are stored in sources.txt to keep CMakeLists clean.
file(STRINGS "${CMAKE_SOURCE_DIR}/sources.txt" SOURCES)
add_executable(pixelfall ${SOURCES})

target_include_directories(pixelfall PRIVATE ${CMAKE_SOURCE_DIR}/src)

if(WIN32)
    target_include_directories(
        pixelfall PRIVATE ${CMAKE_SOURCE_DIR}/external/sdl/include
    )
    target_link_libraries(pixelfall PRIVATE
        ${CMAKE_SOURCE_DIR}/external/sdl/lib/SDL3.lib
        ${CMAKE_SOURCE_DIR}/external/sdl/lib/SDL3_ttf.lib
    )

    # Copy assets folder and .dll files into the build folder.
    add_custom_command(TARGET pixelfall POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/external/sdl/bin/SDL3.dll $<TARGET_FILE_DIR:pixelfall>
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/external/sdl/bin/SDL3_ttf.dll $<TARGET_FILE_DIR:pixelfall>
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:pixelfall>/assets
    )
else()
    find_package(SDL3 REQUIRED)
    find_package(SDL3_ttf REQUIRED)
    target_link_libraries(pixelfall PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)
    
    # Copy assets folder into the build folder.
    add_custom_command(TARGET pixelfall POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:pixelfall>/assets
    )
endif()