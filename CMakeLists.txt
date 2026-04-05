cmake_minimum_required(VERSION 4.3)
project(sampleGame)
set(CMAKE_CXX_STANDARD 17)

# Using Pixelfall Engine
add_subdirectory(pixelfall)

# Sources are stored in sources.txt to keep CMakeLists clean.
file(STRINGS "${CMAKE_SOURCE_DIR}/sources.txt" SOURCES)
add_executable(sampleGame ${SOURCES})

# Define directories used in includes
target_include_directories(sampleGame PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
)

# Link Pixelfall Engine
target_link_libraries(sampleGame PRIVATE pixelfall)

# Copy necessary engine files into the build folder.
add_custom_command(TARGET sampleGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/pixelfall/assets $<TARGET_FILE_DIR:sampleGame>/pixelfall/assets
)

# Windows
if(WIN32)
    # Copy .dll files into the build folder.
    add_custom_command(TARGET sampleGame POST_BUILD
        # Collect DLLs from from pixelfall
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:sampleGame> $<TARGET_FILE_DIR:sampleGame>
        COMMAND_EXPAND_LISTS
    )
endif()