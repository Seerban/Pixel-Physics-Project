#pragma once
#include "Grid.h"
#include <string>
#include <chrono>

class UIHandler {
    int size, scale, font_size;
    Grid *g; // to get data from
    sf::RenderWindow *window; // to render to screen
    sf::Font font;
    sf::Text selection;
    sf::Text temperature;
    sf::Text tps;
public:
    UIHandler(int size, int scale, sf::RenderWindow &window, Grid &g);

    void setTextProp(sf::Text &t);
    void draw();
    int fps();
};