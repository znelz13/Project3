cmake_minimum_required(VERSION 3.22)
project(ChessPuzzleSolver)

add_compile_options(-Wno-deprecated-declarations)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler explicitly (optional, only if needed)
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

# Enable warnings and error on warnings
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

# Fix CLion not finding Boost headers
set(Boost_INCLUDE_DIR "/opt/homebrew/include")
include_directories(${Boost_INCLUDE_DIR})


# Optional: Print Boost include path (for debugging)
message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")

# Include Catch2 via FetchContent (for unit testing)
include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)

# Source includes
include_directories(src)

# Main project executable
add_executable(Main
        src/main.cpp
        src/puzzleLoader.cpp
        src/chessEngineInterface.cpp
        src/solverBacktracking.cpp
        src/solverAStar.cpp
        src/visualization.cpp
        src/utils.cpp
)
