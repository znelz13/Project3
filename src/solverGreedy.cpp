//
// Created by zachary nelson  on 4/10/25.
//

#include "solverGreedy.h"
#include "puzzle.h"
#include "chessEngineInterface.h"

using namespace std;


vector<string> solverGreedy::solvePuzzleGreedy(ChessEngineInterface& engine, const Puzzle &puzzle, int mateInN) {
    vector<string> moves;
    moves.push_back(puzzle.getFirstMove());
    string fen = puzzle.getFen();

    int depth = mateInN * 2;

    for (int algoMoveCount = 0; algoMoveCount < mateInN; ++algoMoveCount) {
        vector<string> legal;
        for (string& m : engine.getLegalMoves(fen)) {
            legal.push_back(m);
        }

        int bestScore = 0;
        string bestMove;
        bool isWhite;
        if (fen.find("w") != string::npos) {
            isWhite = true;
            bestScore = -1000000;
        }
        else {
            isWhite = false;
            bestScore = 1000000;
        }

        for (string& mv : legal) {
            int score = 0;
            string childFen = engine.fenUpdater(fen, mv);
            EvalResult r = engine.evaluatePosition(childFen, depth);
            if (r.mate == true)
                if (isWhite)
                    score = 1000000000;
                else {
                    score = -1000000000;
                }
            else {
                score = r.scoreCp;
            }
            if (isWhite) {
                if (score > bestScore) {
                    bestMove = mv;
                    bestScore = score;
                }
            }
            else {
                if (score < bestScore) {
                    bestMove = mv;
                    bestScore = score;
                }
            }
        }


        moves.push_back(bestMove);
        fen = engine.fenUpdater(fen, bestMove);


        if (engine.isMate(fen)) {
            return moves;
        }


        string reply = engine.bestMove(fen, depth);
        moves.push_back(reply);
        fen = engine.fenUpdater(fen, reply);
    }

    return {};
}



