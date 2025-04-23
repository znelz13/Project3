//
// Created by zachary nelson  on 4/10/25.
//

#include "solverGreedy.h"
#include "puzzle.h"
#include "chessEngineInterface.h"

using namespace std;

vector<string> solverGreedy::solvePuzzleGreedy(
    ChessEngineInterface& engine,
    const Puzzle &puzzle,
    int mateInN
) {
    vector<string> moves;               // solution sequence
    moves.push_back(puzzle.getFirstMove());
    string fen = puzzle.getFen();       // current position

    int depth = mateInN * 4;            // search depth

    for (int step = 0; step < mateInN; ++step) {
        bool isWhite;
        int bestScore;
        if (fen.find("w") != string::npos) {
            isWhite   = true;
            bestScore = -1000000;        // maximize for white
        } else {
            isWhite   = false;
            bestScore =  1000000;        // minimize for black
        }

        vector<string> legal;            // collect legal moves
        for (string& m : engine.getLegalMoves(fen)) {
            legal.push_back(m);
        }

        string bestMove;                 // best candidate

        // evaluate each legal move
        for (string& mv : legal) {
            string childFen = engine.fenUpdater(fen, mv);
            EvalResult r = engine.evaluatePosition(childFen, depth);

            int score;
            int dist = abs(r.scoreMate);

            if (r.mate) {
                // assign large values for mate lines
                if (isWhite) {
                    score = (r.scoreMate <= 0)
                        ? 1000000000 - dist
                        : -1000000000;
                } else {
                    score = (r.scoreMate <= 0)
                        ? -1000000000 + dist
                        :  1000000000;
                }
            } else {
                score = r.scoreCp;       // centipawn score
            }

            // update bestMove based on side
            if (isWhite ? (score > bestScore)
                        : (score < bestScore)) {
                bestMove  = mv;
                bestScore = score;
            }
        }

        moves.push_back(bestMove);
        fen = engine.fenUpdater(fen, bestMove);  // apply best move

        if (engine.isMate(fen)) {
            return moves;               // solution found
        }

        // opponent reply
        string reply = engine.bestMove(fen, depth);
        moves.push_back(reply);
        fen = engine.fenUpdater(fen, reply);
    }

    return {};                         // no mate found
}





