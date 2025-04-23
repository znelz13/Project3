//  puzzleLoader.cpp
//  ----------------
//  Load mate‑in‑N puzzles from lichess_db_puzzle.csv
//
//  You must add to CMakeLists.txt:
//      add_compile_definitions(PROJECT_DIR="${CMAKE_SOURCE_DIR}")
//
//  so PROJECT_DIR expands to the absolute path of your project root.

#include "puzzleLoader.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

vector<Puzzle> loadPuzzlesFromFile(int puzzleSize, int mateInN)
{
    vector<Puzzle> allPuzzles;

    // Absolute path to the CSV
    fs::path csvPath = fs::path(PROJECT_DIR) /
                       "data" /
                       "lichess_db_puzzle.csv";

    ifstream file(csvPath);
    if (!file) {
        cerr << "Could not open file: " << csvPath << '\n';
        return allPuzzles;
    }

    // Read and ignore the header line of the CSV
    string line;
    getline(file, line);


    while (getline(file, line)){
        istringstream lineStream(line);
        string puzzleId, ogFen, moves, rate, theme, garbage;

        getline(lineStream, puzzleId, ',' );
        getline(lineStream, ogFen, ',' );
        getline(lineStream, moves, ',' );
        getline(lineStream, rate, ',' );
        getline(lineStream, garbage, ',' );
        getline(lineStream, garbage, ',' );
        getline(lineStream, garbage, ',' );
        getline(lineStream, theme, ',' );

        // Filter puzzles and only use puzzles where the theme contains "mateInN"
        if (theme.find("mateIn" + to_string(mateInN)) != string::npos){
            int rating = stoi(rate);

            // Extract the first move from the moves string using a string stream.
            string firstMove;
            istringstream iss(moves);
            iss >> firstMove;

            // Create a Puzzle object.
            allPuzzles.emplace_back(puzzleId, "", ogFen, firstMove, rating, theme); // Use emplace_back instead of push_back for performance

            // Stop if we've reached the desired number of puzzles.
            if (allPuzzles.size() == puzzleSize)
                break;
        }
    }
    return allPuzzles;
}

