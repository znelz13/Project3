//
// Created by zachary nelson on 4/10/25.
//

#ifndef PUZZLELOADER_H
#define PUZZLELOADER_H

#include "puzzle.h"
#include <vector>
#include <string>

// Loads puzzles from the CSV dataset file, filtering for mate puzzles, and
// returns up to puzzleSize puzzles as a vector of Puzzle objects.
std::vector<Puzzle> loadPuzzlesFromFile(int puzzleSize, int mateInN);



#endif //PUZZLELOADER_H
