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
#include "Puzzle.h"

class solverBeam {
    vector<vector<string>> solvePuzzleBeam(const Puzzle &puzzle, int beamWidth);
    vector<pair<string, int>> heuristic(const string &fen, int depth);
    vector<string> rankMoves(vector<pair<string, int>> scoredMoves);
};



#endif //solverBeam_H
