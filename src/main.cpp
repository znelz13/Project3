//
// Created by zachary nelson  on 4/10/25.
//
// ------------------------------------------------------------
#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <filesystem>

#include "puzzleLoader.h"
#include "solverBeam.h"
#include "solverGreedy.h"
#include "chessEngineInterface.h"
#include "visualization.h"

int main() {
    // 1. Prompt the user
    int puzzleCount, mateIn;
    std::cout << "How many puzzles to solve? ";
    std::cin  >> puzzleCount;
    std::cout << "Mate in N (e.g. 2): ";
    std::cin  >> mateIn;

    // 2. Load puzzles (no engine calls here)
    auto puzzles = loadPuzzlesFromFile(puzzleCount, mateIn);
    if (puzzles.empty()) {
        std::cerr << "No puzzles loaded; exiting.\n";
        return 1;
    }

    // 3. Create a single Stockfish engine instance
    ChessEngineInterface engine;

    // 4. Apply each puzzle's first move to seed the current FEN
    for (auto &p : puzzles) {
        std::string updatedFen = engine.fenUpdater(p.getOriginalFen(), p.getFirstMove());
        p.setFen(updatedFen);
    }

    // 5. SFML window & textures
    constexpr unsigned tile = 80;
    constexpr unsigned wSize = tile * 8;
    sf::RenderWindow window{
        sf::VideoMode(sf::Vector2u(wSize, wSize)),
        "Chess Puzzle Solver"
    };
    loadPieceTextures();

    // 6. Solve and visualize each puzzle
    for (auto &p : puzzles) {
        const auto id       = p.getPuzzleId();
        const auto startFen = p.getOriginalFen();
        std::cout << "\n=== Puzzle " << id
                  << " (matein" << mateIn << ") ===\n";

        // --- a) Greedy solver ---
        {
            auto t0    = std::chrono::steady_clock::now();
            auto moves = solverGreedy().solvePuzzleGreedy(engine, p, mateIn);
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - t0
            ).count();

            std::cout << "[Greedy] moves: " << moves.size()
                      << ", time: " << elapsed << " ms\n";

            int frame = 0;
            std::string fen = startFen;

            // Frame 0: initial position
            window.clear();
            drawBoard(window);
            drawPieces(window, fen);
            saveFrame(window, frame++);
            window.display();

            // Frames 1..N: each move
            for (const auto &mv : moves) {
                fen = engine.fenUpdater(fen, mv);
                window.clear();
                drawBoard(window);
                drawPieces(window, fen);
                saveFrame(window, frame++);
                window.display();
            }


            generateSolutionGIF("greedy_" + id + ".gif");
        }

        // --- b) Beam‑width‑3 solver ---
        {
            auto   t0    = std::chrono::steady_clock::now();
            auto   moves = solverBeam().solvePuzzleBeam(engine, p, mateIn, /*beamWidth=*/3);
            auto   elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                               std::chrono::steady_clock::now() - t0
                             ).count();

            std::cout << "[Beam‑3] moves: " << moves.size()
                      << ", time: "   << elapsed << " ms\n";

            int         frame = 0;
            std::string fen   = startFen;

            // Frame 0 : initial position
            window.clear();
            drawBoard(window);
            drawPieces(window, fen);
            saveFrame(window, frame++);
            window.display();

            // Frames 1…N : play out the beam solution
            for (const auto& mv : moves) {
                fen = engine.fenUpdater(fen, mv);
                window.clear();
                drawBoard(window);
                drawPieces(window, fen);
                saveFrame(window, frame++);
                window.display();
            }

            generateSolutionGIF("beam_" + id + ".gif");
        }
    }

    unloadPieceTextures();
    return 0;
}















