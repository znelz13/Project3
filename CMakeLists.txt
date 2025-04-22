cmake_minimum_required(VERSION 3.22)
project(ChessPuzzleSolver)

# make PROJECT_DIR available
add_compile_definitions(PROJECT_DIR="${CMAKE_SOURCE_DIR}")

# warnings
add_compile_options(-Wno-deprecated-declarations -Wall -Werror)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force Clang on macOS (optional)
set(CMAKE_C_COMPILER   /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

# -------------- BOOST.INCLUDE + SYSTEM ----------------
set(Boost_INCLUDE_DIR "/opt/homebrew/include")
include_directories(${Boost_INCLUDE_DIR})
message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIR}")

# Add this so you can link Boost::system
find_package(Boost 1.78 REQUIRED COMPONENTS system)
if (Boost_FOUND)
    message(STATUS "Boost.System found: ${Boost_SYSTEM_LIBRARY}")
else()
    message(FATAL_ERROR "Boost.System not found")
endif()

# -------------- SFML 3.0 via Homebrew ------------------
find_package(SFML 3.0 REQUIRED COMPONENTS Graphics Window System)

# -------------- headers & resources --------------------
include_directories("${CMAKE_SOURCE_DIR}/src")
file(COPY "${CMAKE_SOURCE_DIR}/src/res" DESTINATION "${CMAKE_BINARY_DIR}")

# -------------- build Main -----------------------------
add_executable(Main
        src/main.cpp
        src/puzzleLoader.cpp
        src/chessEngineInterface.cpp
        src/solverBeam.cpp
        src/solverGreedy.cpp
        src/visualization.cpp
        src/utils.cpp
)

target_link_libraries(Main
        PRIVATE
        Boost::system
        SFML::Graphics
        SFML::Window
        SFML::System
)



