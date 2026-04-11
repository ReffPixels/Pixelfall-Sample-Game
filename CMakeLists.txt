cmake_minimum_required(VERSION 4.3)
project(chess)
set(CMAKE_CXX_STANDARD 17)

# Using Pixelfall Engine
add_subdirectory(pixelfall)

# Sources are stored in sources.txt to keep CMakeLists clean.
file(STRINGS "${CMAKE_SOURCE_DIR}/sources.txt" SOURCES)
add_executable(chess ${SOURCES})

# Define directories used in includes
target_include_directories(chess PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
)

# Link Pixelfall Engine
target_link_libraries(chess PRIVATE pixelfall)

# Copy necessary engine files into the build folder.
add_custom_command(TARGET chess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/pixelfall/assets $<TARGET_FILE_DIR:chess>/pixelfall/assets
)

# Copy project files into the build folder
add_custom_command(TARGET chess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:chess>/assets
)

# Windows
if(WIN32)
    # Copy .dll files into the build folder.
    add_custom_command(TARGET chess POST_BUILD
        # Collect DLLs from from pixelfall
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:chess> $<TARGET_FILE_DIR:chess>
        COMMAND_EXPAND_LISTS
    )
endif()