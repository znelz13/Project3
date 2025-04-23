#include "visualization.h"
#include <boost/algorithm/string.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>
#include <sstream>
#include <filesystem>

using namespace std;

// Global texture map
static map<char, sf::Texture> pieceTextures;

void unloadPieceTextures()
{
    pieceTextures.clear();
}

void loadPieceTextures() {
    namespace fs = std::filesystem;
    fs::path cppPath   = fs::path(__FILE__);
    fs::path project   = cppPath.parent_path().parent_path();
    fs::path piecesDir = project / "src" / "res" / "pieces";

    map<char, string> textureFiles = {
        {'K', "white_king.png"},   {'Q', "white_queen.png"},
        {'R', "white_rook.png"},   {'B', "white_bishop.png"},
        {'N', "white_knight.png"}, {'P', "white_pawn.png"},
        {'k', "black_king.png"},   {'q', "black_queen.png"},
        {'r', "black_rook.png"},   {'b', "black_bishop.png"},
        {'n', "black_knight.png"}, {'p', "black_pawn.png"},
    };

    for (auto& [symbol, fileName] : textureFiles) {
        sf::Texture tex;
        fs::path fullPath = piecesDir / fileName;
        if (!tex.loadFromFile(fullPath.string())) {
            cerr << "failed to load texture '" << fileName
                 << "' (tried: " << fullPath << ")\n";
        } else {
            pieceTextures[symbol] = std::move(tex);
        }
    }
}

void drawBoard(sf::RenderWindow& window) {
    const float tileSize = 80.f;
    sf::Color light(240, 217, 181);
    sf::Color dark(181, 136,  99);

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
            tile.setPosition(sf::Vector2f(col * tileSize, row * tileSize));
            tile.setFillColor(((row + col) & 1) == 0 ? light : dark);
            window.draw(tile);
        }
    }
}

void drawPieces(sf::RenderWindow& window, const std::string& fen) {
    const float tileSize = 80.f;
    // Extract only the placement portion (before first space)
    size_t pos = fen.find(' ');
    string placement = (pos == string::npos ? fen : fen.substr(0, pos));

    vector<string> ranks;
    boost::split(ranks, placement, boost::is_any_of("/"));
    if (ranks.size() != 8) {
        cerr << "Invalid FEN placement: '" << placement
             << "' (" << ranks.size() << " ranks)\n";
        return;
    }

    for (int row = 0; row < 8; ++row) {
        const auto& rank = ranks[row];
        int col = 0;
        for (char c : rank) {
            if (isdigit(c)) {
                col += (c - '0');
            } else {
                auto it = pieceTextures.find(c);
                if (it != pieceTextures.end()) {
                    sf::Sprite sprite(it->second);
                    // Center the sprite
                    auto texSize = it->second.getSize(); // Vector2u
                    sprite.setOrigin({ texSize.x / 2.f, texSize.y / 2.f });
                    // Position at center of the square
                    sprite.setPosition({
                        col * tileSize + tileSize / 2.f,
                        row * tileSize + tileSize / 2.f
                    });
                    window.draw(sprite);
                }
                ++col;
            }
        }
    }
}

void saveFrame(sf::RenderWindow& window, int frameNumber) {
    auto size = window.getSize();
    sf::Texture texture(sf::Vector2u(size.x, size.y));
    texture.update(window);
    sf::Image screenshot = texture.copyToImage();

    std::ostringstream filename;
    filename << "frame_" << frameNumber << ".png";
    if (!screenshot.saveToFile(filename.str())) {
        cerr << "failed to save frame: " << filename.str() << endl;
    }
}

void generateSolutionGIF(const string& outputFile) {
    string command = "magick -delay 80 -loop 0 frame_*.png " + outputFile;
    if (system(command.c_str()) == 0) {
        cout << "gif generated: " << outputFile << endl;
        for (const auto& entry : filesystem::directory_iterator(".")) {
            auto name = entry.path().filename().string();
            if (name.rfind("frame_", 0) == 0 && entry.path().extension() == ".png") {
                filesystem::remove(entry.path());
            }
        }
    } else {
        cerr << "gif generation failed" << endl;
    }
}



