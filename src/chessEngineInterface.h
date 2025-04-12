//
// Created by zachary nelson  on 4/10/25.
//

#ifndef CHESSENGINEINTERFACE_H
#define CHESSENGINEINTERFACE_H

#include <string>
#include <vector>
#include <boost/process.hpp>

namespace bp = boost::process;
using namespace std;

class ChessEngineInterface {
private:
    boost::process::ipstream engineOutput;
    boost::process::opstream engineInput;
    boost::process::child engine;

    void sendCommand(const string& command);
    void waitForReady();
public:
    ChessEngineInterface();
    ~ChessEngineInterface();

    vector <string> getLegalMoves(const string& fen);
    int evaluatePosition(const string& fen, const string& firstMove, int depth);
    bool isMate(const string& fen);

};



#endif //CHESSENGINEINTERFACE_H
