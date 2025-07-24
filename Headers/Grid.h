#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath> // fabs
#include "Element.h"
#include "Pixel.h"
#include "States.h"
#include "elemutil.h"

const int CHUNK = 4;

class Grid {

    int size; // size of grid on both axis
    int scale; // scale multiplier of window size

    bool even_state = false; // switches process from left-right to right-left
    std::vector<std::vector<Pixel>> grid;
    std::vector<std::vector<bool>> active_chunks;
    std::vector<std::vector<sf::Color>> debug_grid;
    
    sf::Image *image;
    sf::RenderWindow *window;
    public:
    bool debug = false;

    Grid(int size, int scale, int fps, sf::Image &img, sf::RenderWindow &win);
    // processes for pixel interactions
    void process();
    void processPixel(int x, int y);
    void reactionProcess(int x, int y);
    void wetProcess(int x, int y);
    void tempProcess(int x, int y);
    void checkReaction(int x, int y, int x2, int y2, bool overwrite=false);
    // pixel grid functions 
    bool inBounds(int x, int y);
    bool isEmpty(int x, int y);
    elem::Elements getElem(int x, int y);
    float getDensity(int x, int y);
    // manages active chunks
    void setDebug(int x, int y, sf::Color c = sf::Color(0, 55, 0));
    void setChunk(int x, int y, bool b);
    void setChunkRegion(int x, int y);
    void clearDebug();
    void render(int x, int y);
    // pixel manipulation
    void setPixel(int x, int y, elem::Elements s, bool override = false);
    void switchPixel(int x, int y, int x2, int y2);
    // used getters/setters
    double getTemp(int x, int y);
    void setTemp(int x, int y, int val);
    float getWet(int x, int y);
    void setWet(int x, int y, int val);
    sf::Color getCol(int x, int y);
};