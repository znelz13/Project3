//
// Created by zachary nelson  on 4/10/25.
//

#ifndef PUZZLE_H
#define PUZZLE_H

#include <string>
using namespace std;

class Puzzle {
  private:
    string puzzleId;
    string fen;
    string firstMove;
    int rating;
    string theme;
    int mateIn;

  public:
    Puzzle(const string& id, const string& fenStr, const string& firstMoveStr, int rate, const string& themes, int mate)
       : puzzleId(id), fen(fenStr), firstMove(firstMoveStr), rating(rate), theme(themes), mateIn(mate) {}

    // Getters
    string getPuzzleId() const { return puzzleId; }
    string getFen() const { return fen; }
    string getfirstMove() const { return firstMove; }
    int getRating() const { return rating; }
    string getTheme() const { return theme; }
    int getMateIn() const { return mateIn; }
};

#endif //PUZZLE_H
