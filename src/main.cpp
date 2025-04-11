//
// Created by zachary nelson  on 4/10/25.
//
#include <iostream>
#include "puzzle.h"
#include "puzzleLoader.h"

using namespace std;

int main() {

    // Load puzzles
    vector<Puzzle> puzzles = loadPuzzlesFromFile();

    // Test: Print first 5 puzzles
    cout << "Loaded " << puzzles.size() << " puzzles." << endl;

    for (int i = 0; i < 5 && i < puzzles.size(); ++i) {
        cout << "Puzzle " << i + 1 << ":\n";
        cout << "  ID:     " << puzzles[i].getPuzzleId() << endl;
        cout << "  FEN:    " << puzzles[i].getFen() << endl;
        cout << "  Moves:  " << puzzles[i].getMoves() << endl;
        cout << "  Rating: " << puzzles[i].getRating() << endl;
        cout << "  Theme:  " << puzzles[i].getTheme() << "\n" << endl;
    }

    vector<Puzzle> mateIn2Puzzles = filterMateInNPuzzles(puzzles, "mateIn4");

    cout << "Filtered to " << mateIn2Puzzles.size() << " mateIn4 puzzles." << endl;

    return 0;
}
