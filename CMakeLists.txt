cmake_minimum_required(VERSION 3.10)

# Project name and version
project(TerminalOthello VERSION 1.0)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set source and header files
set(SOURCES
    src/main.cpp
    src/utils.cpp
    src/cursor_input.cpp
    src/color.cpp
    src/board.cpp
    src/game.cpp
    src/renderer.cpp
)

set(HEADERS
    src/headers/utils.hpp
    src/headers/cursor_input.hpp
    src/headers/color.hpp
    src/headers/board.hpp
    src/headers/game.hpp
    src/headers/renderer.hpp
)

# Exe
add_executable(Othello ${SOURCES} ${HEADERS})

target_include_directories(Othello PRIVATE src/headers)

# Optional: copy asset folder into build dir
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
