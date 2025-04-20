//
// Created by zachary nelson on 4/10/25.
//
// This header file declares functions for loading chess puzzles from a CSV file
// and for filtering the puzzles based on mate-related themes. The functions use
// the Puzzle class (declared in "puzzle.h") to store information such as the puzzle
// ID, original FEN, the move(s) (e.g., the first move), rating, and theme.
//
// loadPuzzlesFromFile(int puzzleSize):
//   - Opens the puzzle dataset file ("data/lichess_db_puzzle.csv").
//   - Parses each line for puzzle attributes.
//   - Filters out puzzles that are not mate puzzles (by checking the theme).
//   - Limits the number of puzzles loaded to 'puzzleSize'.
//   - Returns a vector of Puzzle objects with the parsed data.
//
// filterMateInNPuzzles(const std::vector<Puzzle>& allPuzzles, const std::string& mateIn):
//   - Examines each Puzzle in the input vector.
//   - Filters and returns only those puzzles whose theme contains the substring 'mateIn'.

#ifndef PUZZLELOADER_H
#define PUZZLELOADER_H

#include "puzzle.h"
#include <vector>
#include <string>

// Loads puzzles from the CSV dataset file, filtering for mate puzzles, and
// returns up to puzzleSize puzzles as a vector of Puzzle objects.
std::vector<Puzzle> loadPuzzlesFromFile(int puzzleSize, int mateInN);



#endif //PUZZLELOADER_H
