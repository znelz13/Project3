//
// Created by zachary nelson  on 4/10/25.
//

#ifndef solverBeam_H
#define solverBeam_H

#include "chessEngineInterface.h"

#include "chessEngineInterface.h"
#include "puzzle.h"
#include <vector>
#include <string>

class solverBeam {
public:
    std::vector<std::string> solvePuzzleBeam(ChessEngineInterface& engine, const Puzzle& puzzle, int mateInN, int beamWidth = 3);
};



#endif //solverBeam_H
