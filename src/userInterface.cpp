#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include "puzzleLoader.h"
#include "solverGreedy.h"
#include "solverBeam.h"
#include "chessEngineInterface.h"
#include "visualization.h"
#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;
// types of puzzles to solve
enum PuzzleType { MATE_IN_1, MATE_IN_2, MATE_IN_3, MATE_IN_4 };
// choice of solving algorithm
enum AlgorithmType { GREEDY, BEAM };

// convert milliseconds to human readable string
static string formatDuration(long long ms) {
    if (ms < 1000) {
        return std::to_string(ms) + " ms";
    }
    long long totalSec = ms / 1000;
    if (totalSec < 60) {

        int tenths = (ms % 1000) / 100;
        return std::to_string(totalSec) + "." + std::to_string(tenths) + " s";
    }
    long long minutes = totalSec / 60;
    long long seconds = totalSec % 60;
    return std::to_string(minutes) + "m " + std::to_string(seconds) + "s";
}

int launchSolverUI() {
    constexpr int windowWidth = 800;    // UI width
    constexpr int windowHeight = 600;   // UI height

    // create main window
    sf::RenderWindow window;
    window.create(sf::VideoMode(sf::Vector2u(windowWidth, windowHeight)),
                  "Chess Puzzle Solver UI");

    // load font for text elements
    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        cerr << "Failed to load font." << endl;
        return 1;
    }
    // user input for puzzle count
    string numPuzzles;
    bool inputActive = false;
    // default puzzle type
    PuzzleType puzzleType = MATE_IN_1;
    // default algorithm
    AlgorithmType algorithm = GREEDY;

    // layout metrics
    float centerX   = windowWidth  / 2.f;
    float baseY     = 80.f;
    float lineHeight= 60.f;

    // title text setup
    sf::Text title(font, "Chess Puzzle Solver", 32);
    title.setStyle(sf::Text::Bold);
    title.setFillColor(sf::Color(40,40,40));
    auto bounds = title.getLocalBounds();
    title.setPosition({ centerX - bounds.size.x/2.f, baseY });

    // label for number of puzzles
    sf::Text numLabel(font, "# Puzzles:", 20);
    numLabel.setFillColor(sf::Color(60,60,60));
    numLabel.setPosition({ centerX-200, baseY+lineHeight });

    // input box for entering puzzle count
    sf::RectangleShape inputBox({120,30});
    inputBox.setPosition({ centerX-40, baseY+lineHeight });
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color(150,150,150));
    inputBox.setOutlineThickness(2);

    // text drawn inside input box
    sf::Text inputText(font, "", 20);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition({ centerX-35, baseY+lineHeight+3 });

    // label for puzzle type selection
    sf::Text puzzleTypeLabel(font, "Puzzle Type:", 20);
    puzzleTypeLabel.setFillColor(sf::Color(60,60,60));
    puzzleTypeLabel.setPosition({ centerX-250, baseY+2*lineHeight });

    // label for algorithm selection
    sf::Text algoLabel(font, "Algorithm:", 20);
    algoLabel.setFillColor(sf::Color(60,60,60));
    algoLabel.setPosition({ centerX-200, baseY+3*lineHeight });

    // option texts for puzzle types and algorithms
    sf::Text mate1(font, "( ) Mate in 1", 20),
             mate2(font, "( ) Mate in 2", 20),
             mate3(font, "( ) Mate in 3", 20),
             mate4(font, "( ) Mate in 4", 20),
             greedy(font, "( ) Greedy",   20),
             beam  (font, "( ) Beam",     20);

    // position option texts
    mate1.setPosition({centerX-120, baseY+2*lineHeight});
    mate2.setPosition({centerX,      baseY+2*lineHeight});
    mate3.setPosition({centerX+120,  baseY+2*lineHeight});
    mate4.setPosition({centerX+240,  baseY+2*lineHeight});
    greedy.setPosition({centerX-20,  baseY+3*lineHeight});
    beam  .setPosition({centerX+120, baseY+3*lineHeight});

    // default color for options
    for (auto *opt : {&mate1,&mate2,&mate3,&mate4,&greedy,&beam})
        opt->setFillColor(sf::Color(30,30,30));

    constexpr float btnW   = 200.f;
    constexpr float btnH   = 45.f;
    constexpr float btnGap = 20.f;
    float btnY = baseY + 4.5f * lineHeight;


    float totalW = btnW * 2 + btnGap;
    float x0     = centerX - totalW * 0.5f;

    // "Start Solving" button
    sf::RectangleShape startButton({ btnW, btnH });
    startButton.setPosition({ x0, btnY });
    startButton.setFillColor(sf::Color(50,180,100));
    startButton.setOutlineColor(sf::Color::Black);
    startButton.setOutlineThickness(1);

    // text on "Start Solving" button
    sf::Text startText(font, "Start Solving", 18);
    startText.setStyle(sf::Text::Bold);
    startText.setFillColor(sf::Color::White);
    {
        sf::FloatRect stb = startText.getLocalBounds();
        float tx = x0 + (btnW - stb.size.x) * 0.5f - stb.position.x;
        float ty = btnY + (btnH - stb.size.y) * 0.5f - stb.position.y;
        startText.setPosition({ tx, ty });
    }

    // "Open Solutions" button
    sf::RectangleShape openBtn({ btnW, btnH });
    openBtn.setPosition({ x0 + btnW + btnGap, btnY });
    openBtn.setFillColor(sf::Color(100,100,250));
    openBtn.setOutlineColor(sf::Color::Black);
    openBtn.setOutlineThickness(1);

    // text on "Open Solutions" button
    sf::Text openText(font, "Open Solutions", 18);
    openText.setFont(font);
    openText.setCharacterSize(18);
    openText.setStyle(sf::Text::Bold);
    openText.setFillColor(sf::Color::White);
    {
        sf::FloatRect otb = openText.getLocalBounds();
        float ox = x0 + btnW + btnGap + (btnW - otb.size.x) * 0.5f - otb.position.x;
        float oy = btnY                         + (btnH - otb.size.y) * 0.5f - otb.position.y;
        openText.setPosition({ ox, oy });
    }

    // "Clear Gifs" button
    constexpr float clearW = 100.f, clearH = 30.f;
    float clearY = btnY + btnH + 10.f;
    sf::RectangleShape clearBtn({ clearW, clearH });
    clearBtn.setFillColor(sf::Color(200,50,50));
    clearBtn.setOutlineColor(sf::Color::Black);
    clearBtn.setOutlineThickness(1);
    clearBtn.setPosition({centerX - clearW*0.5f, clearY});

    // text on "Clear Gifs" button
    sf::Text clearText(font, "Clear Gifs", 14);
    clearText.setStyle(sf::Text::Bold);
    clearText.setFillColor(sf::Color::White);
    {
        auto ctb = clearText.getLocalBounds();
        clearText.setPosition({
            clearBtn.getPosition().x + (clearW - ctb.size.x)*0.5f - ctb.position.x,
            clearBtn.getPosition().y + (clearH - ctb.size.y)*0.5f - ctb.position.y
        });
    }

    // text field for timing results
    sf::Text timeResult(font, "", 18);
    timeResult.setFillColor(sf::Color(40,40,40));

    sf::FloatRect cbb = clearBtn.getGlobalBounds();
    sf::FloatRect ttb = timeResult.getLocalBounds();
    float tx = centerX - (ttb.size.x * 0.5f) - ttb.position.x;
    float ty = cbb.position.y + cbb.size.y + 10.f;
    timeResult.setPosition({ tx, ty });

    // progress bar and status
    sf::RectangleShape progBg({600,20});
    progBg.setFillColor({200,200,200});
    progBg.setPosition({100,520});

    sf::RectangleShape progFg({0,20});
    progFg.setFillColor({50,180,100});
    progFg.setPosition({100,520});

    sf::Text statusText(font, "", 18);  // display timings
    statusText.setFillColor({40,40,40});
    statusText.setPosition({100,550});

    // main loop
    while (window.isOpen()) {
        // event handling
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f m = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                inputActive = inputBox.getGlobalBounds().contains(m);

                // option toggles
                if (mate1.getGlobalBounds().contains(m)) puzzleType = MATE_IN_1;
                if (mate2.getGlobalBounds().contains(m)) puzzleType = MATE_IN_2;
                if (mate3.getGlobalBounds().contains(m)) puzzleType = MATE_IN_3;
                if (mate4.getGlobalBounds().contains(m)) puzzleType = MATE_IN_4;
                if (greedy.getGlobalBounds().contains(m)) algorithm = GREEDY;
                if (beam.getGlobalBounds().contains(m)) algorithm = BEAM;

                // clear gifs
                if (clearBtn.getGlobalBounds().contains(m)) {
                    fs::path solDir = fs::path(PROJECT_DIR) / "src" / "solutions";
                    for (auto &entry : fs::recursive_directory_iterator(solDir)) {
                        if (entry.path().extension() == ".gif") {
                            std::error_code ec;
                            fs::remove(entry.path(), ec);
                            if (ec) std::cerr<<"Failed to delete "<<entry.path()<<": "<<ec.message()<<'\n';
                        }
                    }
                    timeResult.setString("All solutions cleared");

                    {
                        float currentY = timeResult.getPosition().y;
                        auto bound= timeResult.getLocalBounds();
                        float centeredX = centerX - (bound.size.x * 0.5f) - bound.position.x;
                        timeResult.setPosition({ centeredX, currentY });
                    }


                    progFg.setSize({0, progBg.getSize().y});
                }
                // open folder
                if (openBtn.getGlobalBounds().contains(m)) {
                    string solDir = string(PROJECT_DIR) + "/src/solutions";
                    string cmd = "open \"" + solDir + "\"";
                    system(cmd.c_str());
                }
                // start solving
                if (startButton.getGlobalBounds().contains(m)) {
                    try {

                        int count = stoi(numPuzzles);
                        int mate  = (puzzleType==MATE_IN_1?1:
                                     puzzleType==MATE_IN_2?2:
                                     puzzleType==MATE_IN_3?3:4);


                        auto puzzles = loadPuzzlesFromFile(count,mate);
                        if (puzzles.empty()) throw runtime_error("no puzzles");
                        ChessEngineInterface engine;
                        for (auto &p : puzzles)
                            p.setFen(engine.fenUpdater(p.getOriginalFen(),p.getFirstMove()));
                        loadPieceTextures();


                        float totalBarW   = progBg.getSize().x;
                        size_t totalCount = puzzles.size();
                        long long grandTotal = 0;
                        long long lastElapsed = 0;

                        for (size_t i = 0; i < totalCount; ++i) {
                            // update progress bar
                            progFg.setSize({ totalBarW * float(i)/totalCount, progBg.getSize().y });
                            statusText.setString("Solving " + puzzles[i].getPuzzleId() + " (Last Puzzle: " + formatDuration(lastElapsed) + ")");


                            window.clear({240,240,240});
                            window.draw(title);
                            window.draw(numLabel);
                            window.draw(inputBox);
                            window.draw(inputText);
                            window.draw(puzzleTypeLabel);
                            window.draw(mate1);
                            window.draw(mate2);
                            window.draw(mate3);
                            window.draw(mate4);
                            window.draw(algoLabel);
                            window.draw(greedy);
                            window.draw(beam);
                            window.draw(startButton);
                            window.draw(startText);
                            window.draw(timeResult);
                            window.draw(progBg);
                            window.draw(progFg);
                            window.draw(statusText);
                            window.draw(openBtn);
                            window.draw(openText);
                            window.draw(clearBtn);
                            window.draw(clearText);
                            window.display();


                            auto t0 = chrono::steady_clock::now();
                            vector<string> moves = (algorithm==GREEDY)
                                ? solverGreedy().solvePuzzleGreedy(engine,puzzles[i],mate)
                                : solverBeam  ().solvePuzzleBeam  (engine,puzzles[i],mate,3);
                            auto elapsed = chrono::duration_cast<chrono::milliseconds>(
                                chrono::steady_clock::now()-t0).count();
                            lastElapsed = elapsed;
                            grandTotal += elapsed;

                            // generate board frames and GIF
                            constexpr unsigned tile=80, B=tile*8;
                            sf::RenderWindow board;
                            board.create(sf::VideoMode(sf::Vector2u(B,B)), "board", sf::Style::None);

                            int frame=0;
                            string fen=puzzles[i].getOriginalFen();
                            board.clear();
                            drawBoard(board);
                            drawPieces(board,fen);
                            saveFrame(board, frame++);
                            board.display();

                            for (auto &mv: moves) {
                                fen = engine.fenUpdater(fen,mv);
                                board.clear();
                                drawBoard(board);
                                drawPieces(board,fen);
                                saveFrame(board, frame++);
                                board.display();
                            }

                            std::string folder = (algorithm == GREEDY ? "greedy" : "beam");
                            fs::path outFile = fs::path(PROJECT_DIR)/ "src"/ "solutions"/ folder/ (folder + "_" + puzzles[i].getPuzzleId() + ".gif");
                            generateSolutionGIF(outFile.string());

                            // update after each puzzle
                            progFg.setSize({ totalBarW * float(i+1)/totalCount, progBg.getSize().y });
                            statusText.setString("Last Puzzle: " + formatDuration(lastElapsed));

                            window.clear({240,240,240});
                            window.draw(title);
                            window.draw(numLabel);
                            window.draw(inputBox);
                            window.draw(inputText);
                            window.draw(puzzleTypeLabel);
                            window.draw(mate1);
                            window.draw(mate2);
                            window.draw(mate3);
                            window.draw(mate4);
                            window.draw(algoLabel);
                            window.draw(greedy);
                            window.draw(beam);
                            window.draw(startButton);
                            window.draw(startText);
                            window.draw(timeResult);
                            window.draw(progBg);
                            window.draw(progFg);
                            window.draw(statusText);
                            window.draw(openBtn);
                            window.draw(openText);
                            window.draw(clearBtn);
                            window.draw(clearText);
                            window.display();
                        }

                        unloadPieceTextures();
                        timeResult.setString("Total solve time: " + formatDuration(grandTotal));

                        float currentY = timeResult.getPosition().y;
                        {
                            auto bound   = timeResult.getLocalBounds();
                            float centeredX = centerX - (bound.size.x * 0.5f) - bound.position.x;
                            timeResult.setPosition({ centeredX, currentY });
                        }
                    }
                    catch (...) {
                        cerr << "invalid input\n";
                    }
                }
            }

            // text input for number field
            if (event->is<sf::Event::TextEntered>() && inputActive) {
                const auto* te = event->getIf<sf::Event::TextEntered>();
                if (te) {
                    if (te->unicode == '\b') {
                        if (!numPuzzles.empty()) numPuzzles.pop_back();
                    } else if (isdigit(static_cast<char>(te->unicode))) {
                        numPuzzles += static_cast<char>(te->unicode);
                    }
                    inputText.setString(numPuzzles);
                }
            }
        }

        // update option indicators
        mate1.setString((puzzleType==MATE_IN_1?"(o) ":"( ) ") + string("Mate in 1"));
        mate2.setString((puzzleType==MATE_IN_2?"(o) ":"( ) ") + string("Mate in 2"));
        mate3.setString((puzzleType==MATE_IN_3?"(o) ":"( ) ") + string("Mate in 3"));
        mate4.setString((puzzleType==MATE_IN_4?"(o) ":"( ) ") + string("Mate in 4"));
        greedy.setString((algorithm==GREEDY?"(o) ":"( ) ") + string("Greedy"));
        beam.setString((algorithm==BEAM  ?"(o) ":"( ) ") + string("Beam"));

        // redraw UI
        window.clear({240,240,240});
        window.draw(title);
        window.draw(numLabel);
        window.draw(inputBox);
        window.draw(inputText);
        window.draw(puzzleTypeLabel);
        window.draw(mate1);
        window.draw(mate2);
        window.draw(mate3);
        window.draw(mate4);
        window.draw(algoLabel);
        window.draw(greedy);
        window.draw(beam);
        window.draw(startButton);
        window.draw(startText);
        window.draw(timeResult);
        window.draw(progBg);
        window.draw(progFg);
        window.draw(statusText);
        window.draw(openBtn);
        window.draw(openText);
        window.draw(clearBtn);
        window.draw(clearText);
        window.display();
    }

    return 0;
}
















