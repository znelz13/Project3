//
// Created by zachary nelson  on 4/10/25.
//

#include "visualization.h"
#include <boost/algorithm/string.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>
#include <sstream>
#include <filesystem>

using namespace std;

map<char, sf::Texture> pieceTextures;

void loadPieceTextures() {
    // loading files for icons
    map<char, string> textureFiles = {
            {'K', "white_king.png"},   {'Q', "white_queen.png"},
            {'R', "white_rook.png"},   {'B', "white_bishop.png"},
            {'N', "white_knight.png"}, {'P', "white_pawn.png"},
            {'k', "black_king.png"},   {'q', "black_queen.png"},
            {'r', "black_rook.png"},   {'b', "black_bishop.png"},
            {'n', "black_knight.png"}, {'p', "black_pawn.png"},
    };

    for (const auto& [symbol, file] : textureFiles) {
        sf::Texture tex;
        if (!tex.loadFromFile("res/pieces/" + file)) {
            cerr << "failed to load: " << file << endl;
        } else {
            pieceTextures[symbol] = tex;
        }
    }
}

void drawBoard(sf::RenderWindow &window) {
    const int tileSize = 80;
    sf::Color light(240, 217, 181);
    sf::Color dark(181, 136, 99);

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
            tile.setPosition(col * tileSize, row * tileSize);
            tile.setFillColor((row + col) % 2 == 0 ? light : dark);
            window.draw(tile);
        }
    }
}

void drawPieces(sf::RenderWindow &window, const string &fen) {
    const int tileSize = 80;
    vector<string> ranks;
    boost::split(ranks, fen, boost::is_any_of("/"));

    for (int row = 0; row < 8; row++) {
        const string& rank = ranks[row];
        int col = 0;
        for (char c : rank) {
            if (isdigit(c)) {
                col += c - '0';
            } else {
                if (pieceTextures.find(c) != pieceTextures.end()) {
                    sf::Sprite sprite;
                    sprite.setTexture(pieceTextures[c]);
                    sprite.setPosition(col * tileSize, row * tileSize);
                    window.draw(sprite);
                }
                col++;
            }
        }
    }
}

void saveFrame(sf::RenderWindow &window, int frameNumber) {
    sf::Texture texture;
    texture.create(window.getSize().x, window.getSize().y);
    texture.update(window);
    sf::Image screenshot = texture.copyToImage();
    ostringstream filename;
    filename << "frame_" << frameNumber << ".png";
    if (!screenshot.saveToFile(filename.str())) {
        cerr << "failed to save frame: " << filename.str() << endl;
    }
}

void generateSolutionGIF(const string &outputFile) {
    string command = "convert -delay 80 -loop 0 frame_*.png " + outputFile;
    int result = system(command.c_str());
    if (result == 0) {
        cout << "gif generated: " << outputFile << endl;
        for (const auto& entry : filesystem::directory_iterator(".")) {
            std::string filename = entry.path().filename().string();
            if (filename.substr(0, 6) == "frame_" &&
                entry.path().extension() == ".png") {
                filesystem::remove(entry);
            }
        }
    } else {
        cerr << "gif generation failed" << endl;
    }
}

