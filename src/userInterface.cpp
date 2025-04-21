#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <string>
#include <iostream>
#include "puzzleLoader.h"
#include "solverGreedy.h"
#include "solverBeam.h"
#include "chessEngineInterface.h"
#include "visualization.h"

using namespace std;

enum PuzzleType { MATE_IN_1, MATE_IN_2, MATE_IN_3 };
enum AlgorithmType { GREEDY, BEAM };

void runSolver(int puzzleCount, int mateIn, AlgorithmType algorithm) {
    auto puzzles = loadPuzzlesFromFile(puzzleCount, mateIn);
    if (puzzles.empty()) {
        cerr << "No puzzles loaded; exiting.\n";
        return;
    }

    ChessEngineInterface engine;
    for (auto &p : puzzles)
        p.setFen(engine.fenUpdater(p.getOriginalFen(), p.getFirstMove()));

    constexpr unsigned tile = 80;
    constexpr unsigned wSize = tile * 8;
    sf::RenderWindow window;
    window.create(sf::VideoMode({wSize, wSize}), "Chess Puzzle Solver");
    loadPieceTextures();

    for (auto &p : puzzles) {
        const auto id = p.getPuzzleId();
        const auto startFen = p.getOriginalFen();

        vector<string> moves;
        auto t0 = chrono::steady_clock::now();

        if (algorithm == GREEDY)
            moves = solverGreedy().solvePuzzleGreedy(engine, p, mateIn);
        else
            moves = solverBeam().solvePuzzleBeam(engine, p, mateIn);

        auto elapsed = chrono::duration_cast<chrono::milliseconds>(
                chrono::steady_clock::now() - t0).count();

        cout << (algorithm == GREEDY ? "[Greedy]" : "[Beam]")
             << " moves: " << moves.size()
             << ", time: " << elapsed << " ms\n";

        int frame = 0;
        string fen = startFen;
        window.clear();
        drawBoard(window);
        drawPieces(window, fen);
        saveFrame(window, frame++);
        window.display();

        for (const auto& mv : moves) {
            fen = engine.fenUpdater(fen, mv);
            window.clear();
            drawBoard(window);
            drawPieces(window, fen);
            saveFrame(window, frame++);
            window.display();
        }

        string prefix = (algorithm == GREEDY ? "greedy_" : "beam_");
        generateSolutionGIF(prefix + id + ".gif");
    }

    unloadPieceTextures();
}

int launchSolverUI() {
    constexpr int windowWidth = 800;
    constexpr int windowHeight = 600;
    sf::RenderWindow window;
    window.create(sf::VideoMode({windowWidth, windowHeight}), "Chess Puzzle Solver UI");

    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        cerr << "Failed to load font." << endl;
        return 1;
    }

    string numPuzzles = "";
    bool inputActive = false;
    PuzzleType puzzleType = MATE_IN_1;
    AlgorithmType algorithm = GREEDY;

    float centerX = windowWidth / 2.f;
    float baseY = 80.f;
    float lineHeight = 60.f;

    sf::Text title(font, "Chess Puzzle Solver", 32);
    title.setStyle(sf::Text::Bold);
    title.setFillColor(sf::Color(40, 40, 40));
    title.setPosition({centerX - title.getLocalBounds().size.x / 2.f, baseY});

    sf::Text numLabel(font, "# Puzzles:", 20);
    numLabel.setFillColor(sf::Color(60, 60, 60));
    numLabel.setPosition({centerX - 200, baseY + lineHeight});

    sf::RectangleShape inputBox(sf::Vector2f(120, 30));
    inputBox.setPosition({centerX - 40, baseY + lineHeight});
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color(150, 150, 150));
    inputBox.setOutlineThickness(2);

    sf::Text inputText(font, "", 20);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition({centerX - 35, baseY + lineHeight + 3});

    sf::Text puzzleTypeLabel(font, "Puzzle Type:", 20);
    puzzleTypeLabel.setFillColor(sf::Color(60, 60, 60));
    puzzleTypeLabel.setPosition({centerX - 250, baseY + 2 * lineHeight});

    sf::Text algoLabel(font, "Algorithm:", 20);
    algoLabel.setFillColor(sf::Color(60, 60, 60));
    algoLabel.setPosition({centerX - 200, baseY + 3 * lineHeight});

    sf::Text mate1(font, "( ) Mate in 1", 20);
    sf::Text mate2(font, "( ) Mate in 2", 20);
    sf::Text mate3(font, "( ) Mate in 3", 20);
    sf::Text greedy(font, "( ) Greedy", 20);
    sf::Text beam(font, "( ) Beam", 20);
    mate1.setPosition({centerX - 120, baseY + 2 * lineHeight});
    mate2.setPosition({centerX, baseY + 2 * lineHeight});
    mate3.setPosition({centerX + 120, baseY + 2 * lineHeight});
    greedy.setPosition({centerX - 20, baseY + 3 * lineHeight});
    beam.setPosition({centerX + 120, baseY + 3 * lineHeight});

    for (auto* option : {&mate1, &mate2, &mate3, &greedy, &beam}) {
        option->setFillColor(sf::Color(30, 30, 30));
    }

    sf::RectangleShape startButton(sf::Vector2f(200, 45));
    startButton.setPosition({centerX - 100, static_cast<float>(baseY + 4.5 * lineHeight)});
    startButton.setFillColor(sf::Color(50, 180, 100));
    startButton.setOutlineColor(sf::Color::Black);
    startButton.setOutlineThickness(1);

    sf::Text startText(font, "Start Solving", 20);
    startText.setStyle(sf::Text::Bold);
    startText.setFillColor(sf::Color::White);
    startText.setPosition({centerX - 60, static_cast<float>(baseY + 4.5 * lineHeight + 10)});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mouse = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                inputActive = inputBox.getGlobalBounds().contains(mouse);

                if (mate1.getGlobalBounds().contains(mouse)) puzzleType = MATE_IN_1;
                if (mate2.getGlobalBounds().contains(mouse)) puzzleType = MATE_IN_2;
                if (mate3.getGlobalBounds().contains(mouse)) puzzleType = MATE_IN_3;
                if (greedy.getGlobalBounds().contains(mouse)) algorithm = GREEDY;
                if (beam.getGlobalBounds().contains(mouse)) algorithm = BEAM;

                if (startButton.getGlobalBounds().contains(mouse)) {
                    try {
                        int count = stoi(numPuzzles);
                        int mate = (puzzleType == MATE_IN_1) ? 1 : (puzzleType == MATE_IN_2 ? 2 : 3);
                        runSolver(count, mate, algorithm);
                    } catch (...) {
                        cerr << "Invalid numeric input." << endl;
                    }
                }
            }

            if (event->is<sf::Event::TextEntered>() && inputActive) {
                const auto* textEntered = event->getIf<sf::Event::TextEntered>();
                if (textEntered) {
                    if (textEntered->unicode == '\b') {
                        if (!numPuzzles.empty()) numPuzzles.pop_back();
                    } else if (isdigit(static_cast<char>(textEntered->unicode))) {
                        numPuzzles += static_cast<char>(textEntered->unicode);
                    }
                    inputText.setString(numPuzzles);
                }
            }
        }

        mate1.setString((puzzleType == MATE_IN_1 ? "(o) " : "( ) ") + string("Mate in 1"));
        mate2.setString((puzzleType == MATE_IN_2 ? "(o) " : "( ) ") + string("Mate in 2"));
        mate3.setString((puzzleType == MATE_IN_3 ? "(o) " : "( ) ") + string("Mate in 3"));
        greedy.setString((algorithm == GREEDY ? "(o) " : "( ) ") + string("Greedy"));
        beam.setString((algorithm == BEAM ? "(o) " : "( ) ") + string("Beam"));

        window.clear(sf::Color(240, 240, 240));
        window.draw(title);
        window.draw(numLabel);
        window.draw(inputBox);
        window.draw(inputText);
        window.draw(puzzleTypeLabel);
        window.draw(mate1);
        window.draw(mate2);
        window.draw(mate3);
        window.draw(algoLabel);
        window.draw(greedy);
        window.draw(beam);
        window.draw(startButton);
        window.draw(startText);
        window.display();
    }
    return 0;
}
