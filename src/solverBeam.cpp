//
// Created by zachary nelson  on 4/10/25.
//

#include "solverBeam.h"

using namespace std;

vector<string> solverBeam::solvePuzzleBeam(const Puzzle &puzzle, int beamWidth) {
    ChessEngineInterface chessEngineInterface;

    // Fens stores the current iterations positions, newFens is a placeholder that stores the next iterations positions.
    // Once all positions have played their move, fens is set equal to newFens and newFens is cleared
    vector<string> fens;
    vector<string> newFens;

    fens.push_back(chessEngineInterface.fenUpdater(puzzle.getFen(), puzzle.getfirstMove()));
    
    vector<string> legalMoves;

    // Stores the best moves in a position, with bestMoves[0] being the best
    pair<string, int> bestMoves[beamWidth];
    string fen;
    for(int i = puzzle.getMateIn(); i > 0; i++) {
        for(int branches = 0; branches < (int)fens.size(); branches++){

            // Sets bestMoves[0] to the best move in the position
            bestMoves[0].first = chessEngineInterface.bestMove(fens[branches], i * 2);

            legalMoves = chessEngineInterface.getLegalMoves(fens[branches]);

            // Sets the scores of the plaeholders to a bad arbitrary value so that they are immediately replaced
            for(int j = 1; j < beamWidth; j++) {
                bestMoves[j].second = 100000;
            }

            // Checks every possible move for its eval score
            for(int j = 0; j < (int)legalMoves.size(); j++) {

                // Finds the number of moves set by the beamwidth
                for(int k = 1; k < beamWidth; k++) {
                    if(legalMoves[k] != bestMoves[0].first) {
                        int score = chessEngineInterface.evaluatePosition(fen, legalMoves[j], i * 2).scoreCp;
                        if(score < bestMoves[k].second) {

                            // Pushes moves ranked lower down 1
                            for(int l = beamWidth - 1; l > k; l++) {
                                bestMoves[l] = bestMoves[l - 1];
                            }

                            EvalResult eval = chessEngineInterface.evaluatePosition(fen, legalMoves[j], i * 2);
                            bestMoves[k].first = legalMoves[k];
                            bestMoves[k].second = eval.scoreCp;
                        }
                    }
                }
            }

            // Update fens
            for(int j = 0; j < beamWidth; j++) {
                newFens.emplace_back(chessEngineInterface.fenUpdater(fens[branches], bestMoves[j].first));
            }

            // Computer plays best move against
            for(int j = 0; j < beamWidth; j++) {
                newFens.emplace_back(chessEngineInterface.fenUpdater(fens[branches], bestMoves[j].first));
            }
        }
        
        fens = newFens;
        newFens.clear();
    }
}