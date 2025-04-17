//
// Created by zachary nelson  on 4/10/25.
//

#include "solverBeam.h"
#include "puzzle.h"

using namespace std;

vector<string> solverBeam::solvePuzzleBeam(const Puzzle &puzzle, int beamWidth) {
    ChessEngineInterface chessEngineInterface;
    vector<string> fens;
    vector<string> newFins;

    fens.push_back(chessEngineInterface.fenUpdater(puzzle.getfirstMove()));

    vector<string> legalMoves;
    string bestMove;
    pair<string, int> secondBestMove;
    pair<string, int> thirdBestMove;
    string bestMoveFen;
    string secondBestMoveFen;
    string thirdBestMoveFen;
    string fen;
    for(int i = puzzle.getMateIn() * 2 - 1; i > 0; i++) {
        for(int branches = 0; branches < (int)fens.size(); branches++){
            bestMove = chessEngineInterface.BestMove();

            legalMoves = chessEngineInterface.getLegalMoves(fens[branches]);

            secondBestMove.second = 10000000;
            thirdBestMove.second = 10000000;
            for(int j = 0; j < (int)legalMoves.size(); j++) {
                if(legalMoves[j] != bestMove){
                    fen = chessEngineInterface.fenUpdater(legalMoves[j]);
                    if(chessEngineInterface.evaluatePosition(fen, i) < secondBestMove.second){
                        thirdBestMove = secondBestMove;
                        secondBestMove = make_pair<legalMoves[j], chessEngineInterface.evaluatePosition(fen, i)>;
                    }
                    else if(chessEngineInterface.evaluatePosition(fen, i) < thirdBestMove.second){
                        thirdBestMove = make_pair<legalMoves[j], chessEngineInterface.evaluatePosition(fen, i)>;
                    }
                }
            }

            // Update fens
            bestMoveFen = chessEngineInterface.fenUpdater(chessEngineInterface.bestMove(bestMove));
            secondBestMoveFenMoveFen = chessEngineInterface.fenUpdater(chessEngineInterface.bestMove(secondBestMove.first));
            thirdBestMoveFen = chessEngineInterface.fenUpdater(chessEngineInterface.bestMove(thirdBestMove.first));
            
            // Computer plays best move against
            newFins.push_back(chessEngineInterface.fenUpdater(bestMove));
            newFins.push_back(chessEngineInterface.fenUpdater(secondBestMove.first));
            newFins.push_back(chessEngineInterface.fenUpdater(thirdBestMove.first));
        }
    }
}