//
// Created by zachary nelson on 4/10/25.
//

#ifndef CHESSENGINEINTERFACE_H
#define CHESSENGINEINTERFACE_H

#include <string>
#include <vector>
#include <boost/process.hpp>

namespace bp = boost::process;

// EvalResult holds the result of a position evaluation.
//   - If 'mate' is true, then scoreMate represents a mate-in-N value.
//   - If 'mate' is false, then scoreCp holds the centipawn (cp) evaluation.
struct EvalResult {
    bool mate = false;
    int scoreMate = 0;
    int scoreCp = 0;
};

class ChessEngineInterface {
private:
    // Streams for capturing output and feeding input to Stockfish.
    boost::process::ipstream engineOutput;
    boost::process::opstream engineInput;
    boost::process::child    engine;

    // Sends a command string to Stockfish.
    void sendCommand(const std::string& command);
    // Waits for Stockfish to respond with either "uciok" or "readyok".
    void waitForReady();
public:
    // Constructor: Starts Stockfish, sends initialization commands ("uci", "isready") to prepare the engine.
    ChessEngineInterface();
    // Destructor: Sends the "quit" command to Stockfish and waits for it to terminate cleanly.
    ~ChessEngineInterface();

    // Returns a vector of legal moves (in UCI notation) for the given FEN position.
    std::vector <std::string> getLegalMoves(const std::string& fen);
    // Evaluates the position represented by the given FEN at a specified depth.
    // Returns an EvalResult containing mate or cp evaluation values.
    EvalResult evaluatePosition(const std::string& fen, const int depth);
    // Checks if the given FEN position is an immediate checkmate.
    bool isMate(const std::string& fen);
    // Applies a move to the given FEN using Stockfish's output from the dump command ("d").
    // Returns the updated FEN string.
    std::string fenUpdater(const std::string& fen, const std::string& move);
    // Retrieves the best move (in UCI notation) for the given FEN position at the specified depth.
    std::string bestMove(const std::string& fen, const int depth);

};



#endif //CHESSENGINEINTERFACE_H
