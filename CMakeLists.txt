cmake_minimum_required(VERSION 3.22)
project(ChessPuzzleSolver)

# Set C++ standard to C++17 (or adjust as needed)
set(CMAKE_CXX_STANDARD 17)

# Compile flags to match Gradescope test environment (adjust if necessary)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

## Pull in desired version of Catch2 through CMake automatically
include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.5.2  # You can update this version if needed
)
FetchContent_MakeAvailable(Catch2)

# Include the src directory for headers
include_directories(src)

# Main executable for our project
add_executable(Main
        src/main.cpp                    # Entry point; loads puzzles, runs solvers, triggers GIF creation.
        src/puzzleLoader.cpp            # Functions for loading puzzle datasets.
        src/chessEngineInterface.cpp    # Interfaces with Stockfish (getLegalMoves(), isMate(), evaluatePosition()).
        src/solverBacktracking.cpp      # Implements recursive DFS (backtracking) for mate-in-N puzzles.
        src/solverAStar.cpp             # Implements the A* solver (priority queue, heuristic functions, BoardState structure).
        src/visualization.cpp           # Functions for generating GIFs from saved board snapshots.
        src/utils.cpp                   # Utility functions (FEN parsing, move conversion, logging).
)

# Test executable using Catch2 (the tests are in test/test.cpp)
add_executable(Tests
        test/test.cpp                   # Your test file(s)
        src/puzzleLoader.cpp
        src/chessEngineInterface.cpp
        src/solverBacktracking.cpp
        src/solverAStar.cpp
        src/visualization.cpp
        src/utils.cpp
)

# Link the Catch2-provided main to the Tests executable
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)