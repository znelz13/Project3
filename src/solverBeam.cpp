//
// Created by zachary nelson on 4/10/25.
//

#include "chessEngineInterface.h"
#include <solverBeam.h>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct BeamNode {
    vector<string> moves;  // sequence of moves so far
    string         fen;    // current position
    int            score;  // evaluation score
};

vector<string> solverBeam::solvePuzzleBeam(
    ChessEngineInterface& engine,
    const Puzzle& puzzle,
    int mateInN,
    int beamWidth
) {
    vector<BeamNode> beam;
    {
        BeamNode start;
        start.moves.push_back(puzzle.getFirstMove());
        start.fen   = puzzle.getFen();
        start.score = 0;
        beam.push_back(start);  // seed beam with initial move
    }

    int depth = mateInN * 4;  // search depth
    bool isWhite = (puzzle.getFen().find("w") != string::npos);

    // perform beam search over mateInN plies
    for (int step = 0; step < mateInN; ++step) {
        vector<BeamNode> candidates;

        // expand each beam node
        for (BeamNode node : beam) {
            for (string& mv : engine.getLegalMoves(node.fen)) {
                BeamNode child = node;
                child.moves.push_back(mv);
                child.fen = engine.fenUpdater(child.fen, mv);

                EvalResult r = engine.evaluatePosition(child.fen, depth);
                int score;
                int dist = abs(r.scoreMate);

                // assign large weights for mate lines
                if (r.mate) {
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
                    score = r.scoreCp;  // centipawn score
                }

                child.score = score;
                candidates.push_back(child);
            }
        }

        // keep top beamWidth nodes
        sort(candidates.begin(), candidates.end(),
             [](auto const &a, auto const &b){
                 return a.score < b.score;
             });

        vector<BeamNode> nextBeam;
        if (isWhite) {
            // pick highest scores
            for (int i = 0, j = candidates.size() - 1; i < beamWidth; ++i, --j) {
                nextBeam.push_back(candidates[j]);
            }
        } else {
            // pick lowest scores
            for (int i = 0; i < beamWidth; ++i) {
                nextBeam.push_back(candidates[i]);
            }
        }
        beam = nextBeam;

        // check for mate in current beam
        for (auto& n : beam) {
            if (engine.isMate(n.fen)) {
                return n.moves;  // solution found
            }
        }

        // play engine reply to advance positions
        for (auto& n : beam) {
            string reply = engine.bestMove(n.fen, depth);
            n.moves.push_back(reply);
            n.fen = engine.fenUpdater(n.fen, reply);
        }
    }

    return {};  // no solution
}