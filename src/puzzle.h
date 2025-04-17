//
// Created by zachary nelson on 4/10/25.
//
// Puzzle.h
// --------
// This file defines the Puzzle class, which holds all the information related to a chess puzzle.
// Each Puzzle object contains:
//   - A unique puzzle ID
//   - The current FEN string (which may be updated after applying the first move)
//   - The original FEN string (the board state before any moves are applied)
//   - The first move to be played (which is used to update the current FEN)
//   - A rating (reflecting puzzle difficulty/quality)
//   - A theme string (which describes the puzzle, e.g., "mateIn2")
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
    // Constructor: Initializes all member variables using an initializer list.
    // Parameters:
    //   - id: The unique ID of the puzzle.
    //   - fenStr: The current (or updated) FEN string.
    //   - ogFenStr: The original FEN string (unchanged board state).
    //   - firstMoveStr: The first move that starts the puzzle.
    //   - rate: The puzzle rating.
    //   - themes: The theme of the puzzle.
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
