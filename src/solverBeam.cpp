//
// Created by zachary nelson  on 4/10/25.
//

#include "solverBeam.h"

using namespace std;

vector<vector<string>> solverBeam::solvePuzzleBeam(const Puzzle &puzzle, int beamWidth) {
    ChessEngineInterface chessEngineInterface;

    vector<vector<string>> moves;

    // Puts the first move in all branches
    vector<string> firstMove;
    firstMove.push_back(puzzle.getfirstMove());
    vector<pair<string, int>> scoredMoves;
    vector<string> rankedMoves;
    for(int i = 0; i < puzzle.getMateIn() * beamWidth; i++) {
        moves.push_back(firstMove);
    }

    string fen;
    string move;
    string opponentMove;
    int depth = 1;
    for(int mateIn = puzzle.getMateIn(); mateIn > 0; mateIn--) {
        for(int branch = 0; branch < (int)moves.size(); branch += ((int)moves.size() / (depth * beamWidth))) {

            // Gets Fen up to current move
            fen = puzzle.getFen();
            for(int numMoves = 0; numMoves < moves[branch].size(); numMoves++) {
                fen = chessEngineInterface.fenUpdater(fen, moves[branch][numMoves]);
            }

            scoredMoves = heuristic(fen, mateIn);
            rankedMoves = rankMoves(scoredMoves);

            int splitNum = 0;
            for(int split = branch; split < (branch + ((int)moves.size() / (depth * beamWidth))); split += (int)(moves.size() / (beamWidth * beamWidth * depth))) {

                for(int i = split; i < split + (int)(moves.size() / (beamWidth * beamWidth * depth)); i++) {
                    move = rankedMoves[splitNum];

                    // Beam algorithm move
                    moves[i].push_back(move);
                    fen = chessEngineInterface.fenUpdater(fen, move);

                    // Ensures that the opponent doesn't play after mate has been reached
                    if(mateIn == 1) {
                        break;
                    }

                    opponentMove = chessEngineInterface.bestMove(fen, mateIn * 2 - 2);
                    moves[i].push_back(opponentMove);
                }
                splitNum++;
            }
        }
        depth++;
    }

    return moves;
}

vector<pair<string, int>> solverBeam::heuristic(const string &fen, int depth) {
    ChessEngineInterface chessEngineInterface;

    vector<string> legalMoves = chessEngineInterface.getLegalMoves(fen);
    vector<pair<string, int>> scoredMoves;

    EvalResult score;

    string newFen;
    for(int i = 0; i < (int)legalMoves.size(); i++) {
        newFen = chessEngineInterface.fenUpdater(fen, legalMoves[i]);
        score = chessEngineInterface.evaluatePosition(newFen, depth * 2 - 1);
        if(score.mate) {
            scoredMoves.emplace_back(make_pair(legalMoves[i], -10000000));
        }
        else {
            scoredMoves.emplace_back(make_pair(legalMoves[i], score.scoreCp));
        }
    }
    return scoredMoves;
}

vector<string> rankMoves(vector<pair<string, int>> scoredMoves) {
    bool isSorted;
    pair<string, int> placeholder;
    while(!isSorted) {
        isSorted = true;
        for(int i = 0; i < (int)scoredMoves.size() - 1; i++) {
            if(scoredMoves[i].second > scoredMoves[i + 1].second) {
                placeholder = scoredMoves[i];
                scoredMoves[i] = scoredMoves[i + 1];
                scoredMoves[i + 1] = placeholder;
                isSorted = false;
            }
        }
    }

    vector<string> rankedMoves;
    for(int i = 0; i < (int)scoredMoves.size(); i++) {
        rankedMoves.emplace_back(scoredMoves[i].first);
    }

    return rankedMoves;
}