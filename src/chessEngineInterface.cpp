//
// Created by zachary nelson  on 4/10/25.
//

#include "chessEngineInterface.h"
#include <boost/process.hpp>
#include <iostream>
#include <sstream>
#include <cstring>


namespace bp = boost::process;
using namespace std;

// Constructor
ChessEngineInterface::ChessEngineInterface()
    : engine("stockfish", bp::std_out > engineOutput, bp::std_in < engineInput)
{
    sendCommand("uci");
    waitForReady();
    sendCommand("isready");
    waitForReady();
}

// Destructor
ChessEngineInterface::~ChessEngineInterface() {
    engineInput << "quit" << endl;
    engine.wait();
}

void ChessEngineInterface::sendCommand(const string& command) {
    engineInput << command << endl;
}

void ChessEngineInterface::waitForReady() {
    string line;
    while (getline(engineOutput, line)) {
        if (line == "uciok" || line == "readyok")
            break;
    }
}

vector<string> ChessEngineInterface::getLegalMoves(const string& fen) {
    vector<string> legalMoves;

    sendCommand("position fen " + fen);
    sendCommand("go perft 1");

    string line;
    while (getline(engineOutput, line)) {
        if (line.find("info string", 0) == 0)
            continue;
        if (line.find("Nodes searched:") != string::npos)
            break;
        int colon = line.find(":");
        if (colon != string::npos) {
            string move = line.substr(0, colon);
            legalMoves.push_back(move);
        }
    }

    return legalMoves;
}




// Evaluate the position's score from Stockfish
// remember for future implementation depth param needs to be 2x puzzle to find mate (mate in 2 depth has to be 4)
int ChessEngineInterface::evaluatePosition(const string& fen, const string& firstMove, int depth) {

    sendCommand("position fen " + fen + " moves " + firstMove);
    sendCommand("go depth " + to_string(depth));

    string line;
    while (getline(engineOutput, line)) {
        if (line.find("info string", 0) == 0)
            continue;
        if (line.find("score mate") != string::npos) {
            istringstream iss(line);
            string token;

            while (iss >> token) {
                if (token == "mate") {
                    int mate;
                    iss >> mate;
                    return mate;
                }
            }
        }
    }
    return 100;
}

// Check if a position is checkmate
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
    }
    return false;
}

