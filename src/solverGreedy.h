//
// Created by zachary nelson  on 4/10/25.
//

#ifndef solverGreedy_H
#define solverGreedy_H

#include <vector>
#include <string>
#include "puzzle.h"
#include "chessEngineInterface.h"

class solverGreedy {
public:
    std::vector<std::string> solvePuzzleGreedy(ChessEngineInterface& engine, const Puzzle &puzzle, int mateInN);
};



#endif //solverGreedy_H
