//
// Created by zachary nelson  on 4/10/25.
//

#ifndef PUZZLELOADER_H
#define PUZZLELOADER_H

#include "puzzle.h"
#include <vector>
#include <string>

using namespace std;


vector<Puzzle> loadPuzzlesFromFile();
vector<Puzzle> filterMateInNPuzzles(const vector<Puzzle>& puzzles, const string& theme);





#endif //PUZZLELOADER_H
