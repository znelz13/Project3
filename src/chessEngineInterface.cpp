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
}


// Destructor
ChessEngineInterface::~ChessEngineInterface() {
    // TODO: Close any open streams
}

void ChessEngineInterface::sendCommand(const string& command) {
    engineInput << command << endl;
}

void ChessEngineInterface::waitForReady() {

}

string ChessEngineInterface::readResponse() {

}


vector<string> ChessEngineInterface::getLegalMoves(const string& fen) {
    vector<string> moves;
}




// Evaluate the position's score from Stockfish
int ChessEngineInterface::evaluatePosition(const string& fen) {
    // TODO: Use "go depth 1" and extract score cp or score mate
    return 0;
}

// Check if a position is checkmate
bool ChessEngineInterface::isMate(const string& fen) {
    // TODO: Look for "score mate 0"
    return false;
}

