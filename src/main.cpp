//
// Created by zachary nelson  on 4/10/25.
//
#include <iostream>
#include "puzzle.h"
#include "puzzleLoader.h"
#include "chessEngineInterface.h"

using namespace std;

int main() {

    vector<Puzzle> allPuzzles = loadPuzzlesFromFile();

    // Just print the first 10 puzzles for verification
    for (int i = 0; i < 10 && i < allPuzzles.size(); ++i) {
        const Puzzle& puzzle = allPuzzles[i];
        cout << "Puzzle ID: " << puzzle.getPuzzleId() << endl;
        cout << "FEN: " << puzzle.getFen() << endl;
        cout << "First Move: " << puzzle.getfirstMove() << endl;
        cout << "Rating: " << puzzle.getRating() << endl;
        cout << "Theme: " << puzzle.getTheme() << endl;
        cout << "------------------------" << endl;
    }

}
