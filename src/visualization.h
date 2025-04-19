//
// Created by zachary nelson  on 4/10/25.
//

#ifndef VISUALIZATION_H
#define VISUALIZATION_H
#include <SFML/Graphics.hpp>
#pragma once

void loadPieceTextures();
void drawBoard(sf::RenderWindow &window);
void drawPieces(sf::RenderWindow &window, const std::string &fen);
void saveFrame(sf::RenderWindow &window, int frameNumber);
void generateSolutionGIF(const std::string &outputFile);

#endif //VISUALIZATION_H
