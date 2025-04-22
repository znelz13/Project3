//
// Created by zachary nelson  on 4/10/25.
//

#include "chessEngineInterface.h"
#include <solverBeam.h>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct BeamNode {
    vector<string> moves;
    string         fen;
    int            score;
};

vector<string> solverBeam::solvePuzzleBeam(ChessEngineInterface& engine, const Puzzle& puzzle, int mateInN, int beamWidth) {
    vector<BeamNode> beam;
    {
        BeamNode start;
        start.moves.push_back(puzzle.getFirstMove());
        start.fen = puzzle.getFen();
        start.score = 0;
        beam.push_back(start);
    }

    int depth = mateInN * 4;
    bool isWhite;
    if (puzzle.getFen().find("w") != string::npos) {
        isWhite = true;
    }
    else {
        isWhite = false;
    }

    for (int algoMoveCount = 0; algoMoveCount < mateInN; ++algoMoveCount) {
        vector<BeamNode> childrenCandidates;
        for (BeamNode beamNode : beam) {
            for (string& mv : engine.getLegalMoves(beamNode.fen)) {
                BeamNode child = beamNode;
                child.moves.push_back(mv);
                child.fen = engine.fenUpdater(child.fen, mv);
                EvalResult r = engine.evaluatePosition(child.fen, depth);

                int score = 0;
                int distanceMate = abs(r.scoreMate);
                if (r.mate) {
                    if (isWhite) {
                        if (r.scoreMate <= 0) {
                            score = 1000000000 - distanceMate;
                        } else {
                            score = -1000000000;
                        }
                    } else {
                        if (r.scoreMate <= 0) {
                            score = -1000000000 + distanceMate;
                        } else {
                            score = 1000000000;
                        }
                    }
                }
                else {
                    score = r.scoreCp;
                }
                child.score = score;
                childrenCandidates.push_back(child);
            }
        }

        // Sorts smallest to largest
        sort(childrenCandidates.begin(),
          childrenCandidates.end(),
          [](auto const &a, auto const &b){
            return a.score < b.score;
          });

        vector <BeamNode> newBeamNode;
        if (isWhite) {
            int childrenCandidatesSize = childrenCandidates.size() - 1;
            for (int i = 0; i < beamWidth; ++i) {
                newBeamNode.push_back(childrenCandidates[childrenCandidatesSize]);
                childrenCandidatesSize--;
            }
        }
        else {
            for (int i = 0; i < beamWidth; ++i) {
                newBeamNode.push_back(childrenCandidates[i]);
            }
        }
        beam = newBeamNode;

        for (BeamNode& node : beam) {
            if (engine.isMate(node.fen)) {
                return node.moves;
            }
        }

        for (BeamNode& node : beam) {
            string reply = engine.bestMove(node.fen, depth);
            node.moves.push_back(reply);
            string fen = engine.fenUpdater(node.fen, reply);
            node.fen = fen;

        }
    }
    return {};

}