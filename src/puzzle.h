//
// Created by zachary nelson on 4/10/25.
//


#ifndef PUZZLE_H
#define PUZZLE_H

#include <string>

class Puzzle {
  private:
    // Unique identifier for the puzzle.
    std::string puzzleId;
    // The current FEN string that is updated with the first move.
    std::string fen;
    // The original FEN string of the puzzle, which remains unchanged.
    std::string originalFen;
    // The first move (in UCI notation) that should be played to begin the puzzle.
    std::string firstMove;
    // A numerical rating of the puzzle (e.g., difficulty or quality score).
    int rating;
    // A theme string that may include tags such as "mateIn2", "endgame", etc.
    std::string theme;

  public:
    // Constructor that initializes all member variables using an initializer list.
    Puzzle(const std::string& id, const std::string& fenStr, const std::string& ogFenStr, const std::string& firstMoveStr, int rate, const std::string& themes)
       : puzzleId(id), fen(fenStr), originalFen(ogFenStr), firstMove(firstMoveStr), rating(rate), theme(themes) {}

    // Getters to access the private members.
    std::string getPuzzleId() const { return puzzleId; }
    std::string getFen() const { return fen; }
    std::string getOriginalFen() const { return originalFen; }
    std::string getFirstMove() const { return firstMove; }
    int getRating() const { return rating; }
    std::string getTheme() const { return theme; }

    // Setter for updating the current FEN.
    void setFen(const std::string& updatedFen) { fen = updatedFen; }
};

#endif //PUZZLE_H
