//
// Created by zachary nelson  on 4/10/25.
//

#include "chessEngineInterface.h"
#include <boost/process.hpp>
#include <iostream>
#include <sstream>


namespace bp = boost::process;
using namespace std;

/*
 * Constructor:
 *  - Launches the Stockfish engine using Boost.Process.
 *  - Sends the "uci" command to initialize Stockfish, and waits for the engine to respond with "uciok".
 *  - Sends the "isready" command and waits for the "readyok" response.
 */
ChessEngineInterface::ChessEngineInterface()
    : engine("stockfish", bp::std_out > engineOutput, bp::std_in < engineInput)
{
    sendCommand("uci");
    waitForReady();
    sendCommand("isready");
    waitForReady();
}

/*
 * Destructor:
 *  - Sends the "quit" command to Stockfish to terminate the engine.
 *  - Calls engine.wait() to block until the Stockfish process has finished terminating.
 */
ChessEngineInterface::~ChessEngineInterface() {
    engineInput << "quit" << endl;
    engine.wait();
}

/*
 * sendCommand
 *  - Sends a command string to the Stockfish engine.
 *  - Appends an end-of-line character, as required by Stockfish.
 */
void ChessEngineInterface::sendCommand(const string& command) {
    engineInput << command << endl;
}

/*
 * waitForReady
 *  - Reads lines from Stockfish until it receives either "uciok" or "readyok",
 *    which indicates that Stockfish is ready to process further commands.
 */
void ChessEngineInterface::waitForReady() {
    string line;
    while (getline(engineOutput, line)) {
        if (line == "uciok" || line == "readyok")
            break;
    }
}

/*
 * getLegalMoves
 *  - Sends the current FEN to Stockfish and issues "go perft 1" to list legal moves.
 *  - Filters out lines starting with "info string" and stops once "Nodes searched:" is seen.
 *  - Extracts each move from a line by splitting on the colon and stores it in the vector.
 *  - Returns a vector of legal move strings.
 */
vector<string> ChessEngineInterface::getLegalMoves(const string& fen) {
    vector<string> legalMoves;

    sendCommand("position fen " + fen);
    sendCommand("go perft 1");

    string line;
    while (getline(engineOutput, line)) {
        // Skip informational lines
        if (line.find("info string", 0) == 0)
            continue;
        // Stop if we see the final summary line (e.g., "Nodes searched:")
        if (line.find("Nodes searched:") != string::npos)
            break;
        // If the line contains a colon, assume it's a move output in the form "move: count"
        int colon = line.find(":");
        if (colon != string::npos) {
            string move = line.substr(0, colon); // Extract move by taking substring up to colon
            legalMoves.push_back(move);
        }
    }

    return legalMoves;
}

/*
 * evaluatePosition
 *  - Sends the given FEN position to Stockfish and a "go depth" command to evaluate it.
 *  - Reads the engine's output, capturing the last score-containing line.
 *  - Parses the line to check if the score is for a mate (using the token "mate") or a centipawn evaluation (token "cp").
 *  - Returns an EvalResult struct with the appropriate values.
 *
 * Note:
 *  - For forced mate puzzles, you need to use a depth that is twice the expected mate number.
 */
EvalResult ChessEngineInterface::evaluatePosition(const string& fen, const int depth) {

    sendCommand("position fen " + fen);
    sendCommand("go depth " + to_string(depth));

    string line;
    string lastScore;
    EvalResult result;
    // Read through Stockfish's output and save the last line containing "score"
    while (getline(engineOutput, line)) {
        if (line.find("info string") == 0)
            continue;
        if (line.find("score") != string::npos) {
            lastScore = line;
        }
        // Break once we see the "bestmove" line, indicating the evaluation is complete.
        if (line.find("bestmove") != string::npos) {
            break;
        }
    }

    // If a score was found, parse it.
    if (!lastScore.empty()) {
        istringstream iss(lastScore);
        string token;
        while (iss >> token) {
            if (token == "mate") {
                iss >> result.scoreMate;
                result.mate = true;
            }

            if (token == "cp") {
                iss >> result.scoreCp;
            }
        }
    }

    return result;
}

/*
 * isMate
 *  - Checks if the given FEN position is an immediate checkmate.
 *  - Sends the FEN to Stockfish and uses a shallow search (depth 1) to determine if mate is present.
 *  - Returns true if Stockfish outputs "score mate 0"; otherwise, returns false.
 */
bool ChessEngineInterface::isMate(const string& fen) {

    sendCommand("position fen " + fen);
    sendCommand("go depth 1");

    string line;
    while (getline(engineOutput, line)) {
        if (line.find("info string", 0) == 0)
            continue;
        if (line.find("score mate 0") != string::npos) {
            return true;
        }
        // Break if bestmove is encountered, to avoid blocking indefinitely.
        if (line.find("bestmove") != string::npos) {
            break;
        }
    }
    return false;
}

/*
 * fenUpdater
 *  - Applies a move to a given FEN by sending it to Stockfish using the UCI command "position fen <fen> moves <move>".
 *  - Then, it issues the "d" command to dump the board state.
 *  - Parses the output for a line starting with "Fen:" and returns the substring that follows.
 *  - This updated FEN string represents the new board state after the move is applied.
 */
string ChessEngineInterface::fenUpdater(const string& fen, const string& move) {
    sendCommand("position fen " + fen + " moves " + move);
    sendCommand("d");

    string line;
    while (getline(engineOutput, line)) {
        if (line.find("Fen:") != string::npos) {
            // Extract the updated FEN by skipping past "Fen:" (which is 5 characters including the space).
            return line.substr(line.find("Fen:") + 5);
        }
    }
    return "";
}

/*
 * bestMove
 *  - Determines the best move for the given FEN position by sending a "go depth" command to Stockfish.
 *  - It filters out lines starting with "info string" or "info depth", then locates the line containing "bestmove".
 *  - Parses that line to extract and return the best move in UCI notation.
 */
string ChessEngineInterface::bestMove(const string& fen, const int depth) {
    sendCommand("position fen " + fen);
    sendCommand("go depth " + to_string(depth));

    string line;
    while (getline(engineOutput, line)) {
        // Skip lines with irrelevant info
        if (line.find("info string") == 0 || line.find("info depth") == 0)
            continue;
        // When "bestmove" is found, parse and return it.
        if (line.find("bestmove") != string::npos) {
            istringstream iss(line);
            string token;
            while (iss >> token) {
                if (token == "bestmove") {
                    string bestMove;
                    iss >> bestMove;
                    return bestMove;
                }
            }
        }
    }
    return "";
}