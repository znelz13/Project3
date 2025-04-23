//
// Created by Amit Sagar on 4/20/25.
//

#ifndef CHESSPUZZLESOLVER_USERINTERFACE_H
#define CHESSPUZZLESOLVER_USERINTERFACE_H

#ifndef CHESS_UI_H
#define CHESS_UI_H

enum PuzzleType {
    MATE_IN_1,
    MATE_IN_2,
    MATE_IN_3
};

enum AlgorithmType {
    GREEDY,
    BEAM
};

int launchSolverUI();



#endif // CHESS_UI_H


#endif //CHESSPUZZLESOLVER_USERINTERFACE_H
