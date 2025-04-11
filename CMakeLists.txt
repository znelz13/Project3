 cmake_minimum_required(VERSION 3.22)
 project(ChessPuzzleSolver)

 set(CMAKE_C_COMPILER /usr/bin/clang)
 set(CMAKE_CXX_COMPILER /usr/bin/clang++)

 set(CMAKE_CXX_STANDARD 17)
 set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror")
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

 include(FetchContent)
 FetchContent_Declare(
         Catch2
         GIT_REPOSITORY https://github.com/catchorg/Catch2.git
         GIT_TAG        v3.5.2
 )
 FetchContent_MakeAvailable(Catch2)

 include_directories(src)

 add_executable(Main
         src/main.cpp
         src/puzzleLoader.cpp
         src/chessEngineInterface.cpp
         src/solverBacktracking.cpp
         src/solverAStar.cpp
         src/visualization.cpp
         src/utils.cpp
 )
