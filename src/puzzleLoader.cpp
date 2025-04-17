//
// Created by zachary nelson on 4/10/25.
//
// puzzleLoader.cpp
// -----------------
// This file contains functions for loading chess puzzles from the CSV dataset and
// filtering them based on mate themes. The puzzles are parsed from a CSV file, where
// each puzzle is represented by its puzzleId, original FEN string, moves string, rating,
// and theme (with some extra columns that are ignored). Only puzzles whose theme contains
// "mate" are added. For each selected puzzle, the first move is extracted and later used
// to update the FEN (using the ChessEngineInterface) to reflect the state after the first move.
//
// Functions:
//   - loadPuzzlesFromFile(int puzzleSize):
//       Reads the CSV file "data/lichess_db_puzzle.csv", parses each puzzle line,
//       filters for mate puzzles, extracts the first move from the moves string, and
//       returns a vector of Puzzle objects (up to puzzleSize puzzles).
//
//   - filterMateInNPuzzles(const std::vector<Puzzle>& allPuzzles, const std::string& mateIn):
//       Filters the given vector of puzzles and returns only those puzzles whose theme
//       contains the provided mateIn string.

#include "chessEngineInterface.h"
#include "puzzleLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

/*
 * Loads puzzles from a CSV file, filtering for mate puzzles, and returns a vector of Puzzle objects.
 *
 * Parameters:
 *    - puzzleSize: The maximum number of puzzles to load.
 *
 * Process:
 *    1. Opens the CSV file ("data/lichess_db_puzzle.csv") and checks for errors.
 *    2. Skips the header line.
 *    3. For each line, parses key fields: puzzleId, original FEN (ogFen), moves, rating, and theme.
 *    4. Extracts the first move from the moves string only for puzzles with "mate" in the theme.
 *    5. Constructs a Puzzle object (initially leaving the updated FEN empty) and adds it to the vector.
 *    6. Limits the loaded puzzles to 'puzzleSize'.
 *    7. After loading, creates one instance of ChessEngineInterface and updates the FEN for every puzzle
 *       using the fenUpdater() method so that the puzzle's current FEN reflects the state after the first move.
 *
 * Returns:
 *    A vector of Puzzle objects.
 */
vector<Puzzle> loadPuzzlesFromFile(int puzzleSize)
{

  // Create a vector of puzzle objects
  vector<Puzzle> allPuzzles;

  // Open the puzzle CSV file
  ifstream file("data/lichess_db_puzzle.csv");
  if (!file){
    cerr << "Could not open file " << endl;
    return allPuzzles;
  }

  // Read and ignore the header line of the CSV
  string line;
  getline(file, line);

  // Process each line of the CSV file
  while (getline(file, line)){
    istringstream lineStream(line);
    string puzzleId, ogFen, moves, rate, theme, garbage;

    // Parse the expected columns (puzzleId, originalFen, moves, rating, 3 columns of garbage, then theme)
    getline(lineStream, puzzleId, ',' );
    getline(lineStream, ogFen, ',' );
    getline(lineStream, moves, ',' );
    getline(lineStream, rate, ',' );
    getline(lineStream, garbage, ',' );
    getline(lineStream, garbage, ',' );
    getline(lineStream, garbage, ',' );
    getline(lineStream, theme, ',' );

    // Filter puzzles: Only use puzzles where the theme contains "mate"
    if (theme.find("mate") != string::npos){
      int rating = stoi(rate);

      // Extract the first move from the moves string using a string stream.
      string firstMove;
      istringstream iss(moves);
      iss >> firstMove;

      // Create a Puzzle object.
      // Arguments: puzzleId, current FEN (empty here, will update later), original FEN, first move, rating, theme.
      allPuzzles.emplace_back(puzzleId, "", ogFen, firstMove, rating, theme); // Use emplace_back instead of push_back for performance

      // Stop if we've reached the desired number of puzzles.
      if (allPuzzles.size() == puzzleSize)
        break;
    }
  }

  // Create a single Stockfish engine instance to update the FEN for each puzzle.
  ChessEngineInterface engine;
  // For each loaded puzzle, update the FEN by applying the first move.
  for (Puzzle& p : allPuzzles) {
    // engine.fenUpdater takes the original FEN and the first move, returning the updated FEN.
    string updatedFen = engine.fenUpdater(p.getOriginalFen(), p.getFirstMove());
    p.setFen(updatedFen);   // Set the updated FEN in the puzzle.
  }

return allPuzzles;
}

/*
 * Filters a vector of Puzzle objects and returns only those puzzles
 * whose theme contains the given mateIn substring.
 *
 * Parameters:
 *    - allPuzzles: The vector of Puzzle objects to filter.
 *    - mateIn: The substring used to filter puzzles (e.g., "mateIn2").
 *
 * Returns:
 *    A vector of Puzzle objects that have mateIn in their theme.
 */
vector<Puzzle> filterMateInNPuzzles(const vector<Puzzle>& allPuzzles, const string& mateIn)
{
  vector<Puzzle> filteredPuzzles;
  for (const Puzzle& puzzle : allPuzzles) {
    if (puzzle.getTheme().find(mateIn) != string::npos)
      filteredPuzzles.push_back(puzzle);
  }
  return filteredPuzzles;
}