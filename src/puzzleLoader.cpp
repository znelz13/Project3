//
// Created by zachary nelson  on 4/10/25.
//

#include "puzzleLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

/* Function that opens the file that contains the database of chess puzzles, grabs all relevant data,
 * checks whether the puzzle is a mate puzzle then adds it to the database of all puzzles */
vector<Puzzle> loadPuzzlesFromFile()
{

  // Create a vector of puzzle objects and opens puzzle database file
  vector<Puzzle> allPuzzles;
  ifstream file("data/lichess_db_puzzle.csv");

  // Checks for error loading file
  if (!file){
    cerr << "Could not open file " << endl;
  }

  // Gets the header line before the while loop
  string line;
  getline(file, line);

  // Loop to get all the information needed from each puzzle and stores it in the all puzzle database
  while (getline(file, line)){
    istringstream lineStream(line);
    string puzzleId, fen, moves, rate, theme, garbage;

    getline(lineStream, puzzleId, ',' );
    getline(lineStream, fen, ',' );
    getline(lineStream, moves, ',' );
    istringstream moveStream(moves);
    string firstMove;
    moveStream >> firstMove;

    getline(lineStream, rate, ',' );
    getline(lineStream, garbage, ',' );
    getline(lineStream, garbage, ',' );
    getline(lineStream, garbage, ',' );
    getline(lineStream, theme, ',' );

    int rating = stoi(rate);

    // checks if puzzle is mate puzzle before adding it in
    if (theme.find("mate") != string::npos){
      allPuzzles.emplace_back(puzzleId, fen, firstMove, rating, theme); // Use emplace_back instead of push_back for performance

      // Controls how many puzzles are added to the vector
      if (allPuzzles.size() == 250000)
        break;
    }
  }

return allPuzzles;
}

vector<Puzzle> filterMateInNPuzzles(const vector<Puzzle>& allPuzzles, const string& mateIn)
{
  vector<Puzzle> filteredPuzzles;
  for (const Puzzle& puzzle : allPuzzles) {
    if (puzzle.getTheme().find(mateIn) != string::npos)
      filteredPuzzles.push_back(puzzle);
  }
  return filteredPuzzles;
}