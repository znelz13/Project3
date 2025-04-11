//
// Created by zachary nelson  on 4/10/25.
//
#include <iostream>
#include "puzzle.h"
#include "puzzleLoader.h"
#include "chessEngineInterface.h"

using namespace std;

int main() {

    ChessEngineInterface engine;

    string testFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    vector<string> moves = engine.getLegalMoves(testFEN);

    cout << "Legal moves:\n";
    for (const string& move : moves) {
        cout << move << " ";
    }
    cout << endl;

    return 0;

}
