//
// Created by zachary nelson  on 4/10/25.
//

#ifndef solverBeam_H
#define solverBeam_H

#include "chessEngineInterface.h"

#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include "puzzle.h"

class solverBeam {
public:
    std::vector<std::vector<std::string>> solvePuzzleBeam(const Puzzle &puzzle, int mateInN);
};



#endif //solverBeam_H
