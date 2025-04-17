//
// Created by zachary nelson  on 4/10/25.
//

#include "solverGreedy.h"

using namespace std;

vector<string> solverGreedy::solvePuzzleGreedy(const Puzzle &puzzle) {
    ChessEngineInterface chessEngineInterface;
    vector<string> moves;
    string fen;

    // Plays the first move
    fen = chessEngineInterface.fenUpdater(puzzle.getFen(), puzzle.getfirstMove());
    moves.push_back(puzzle.getfirstMove());

    string bestMove;
    string opponentMove;
    for(int i = puzzle.getMateIn(); i > 0; i--) {
        // Greedy algorithm move
        bestMove = chessEngineInterface.bestMove(fen, i * 2 - 1);
        moves.push_back(bestMove);
        fen = chessEngineInterface.fenUpdater(fen, bestMove);

        // Ensures that the opponent doesn't play after mate has been reached
        if(chessEngineInterface.isMate(fen)) {
            return moves;
        }

        opponentMove = chessEngineInterface.bestMove(fen, i * 2 - 2);
        moves.push_back(opponentMove);
        fen = chessEngineInterface.fenUpdater(fen, opponentMove);
    }

    // Returns an empty vector if mate is not reached
    vector<string> fail;
    return fail;
}