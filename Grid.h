#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include <random>
#include <utility>
#include "Element.h"
#include "Pixel.h"

float randf();

class Grid {
    static int size; // size of grid on both axis
    static int scale; // scale multiplier of window size
    static std::vector<std::vector<Pixel>> grid;
    static sf::Image image;
    static sf::Texture texture;
    static sf::Sprite sprite;
    static sf::RenderWindow window;
    static std::vector<void(*)(int,int)> stateProcess; // inititalized in element.cpp
    public:
    // constructors
    Grid() : Grid(40) {}
    Grid(int size, int scale = 8) {
            this->size = size;
            this->scale = scale;
            window.create( sf::VideoMode(size*scale, size*scale), "Grid" );
            grid.resize(size, std::vector<Pixel>(size));
            image.create(size, size, sf::Color::Black);
            texture.loadFromImage(image);
            sprite.setTexture(texture);
            sprite.setScale(scale, scale);
            window.setFramerateLimit(12);
        }
    // main process functions
    void start() {
        srand(time(0));

        while (window.isOpen()) {
            handleInput();

            mainProcess();
            texture.update(image); // should be replace to update only 1 pixel but newest sfml incompatible with my system

            window.clear();
            window.draw(sprite);
            window.display();
        }
    }
    void mainProcess() {
        // bottom-to-top loop for processing liquids/dusts
        for(int i = size-1; i >= 0; --i) {
            for( int j = 0; j < size; ++j )
                if( grid[i][j].getState() != elem::GAS && !grid[i][j].getProcessed() ) {
                    grid[i][j].setProcessed(true);
                    // movement utility functions defined in grid.cpp
                    reactionProcess(i, j);
                    (stateProcess[ grid[i][j].getState() ])(j, i);
                }
        }
        // top-to-bottom loop for processing gas
        for(int i = 0; i < size; ++i) {
            for( int j = 0; j < size; ++j )
                if( grid[i][j].getState() == elem::GAS && !grid[i][j].getProcessed() ) {
                    grid[i][j].setProcessed(true);
                    // movement utility functions defined in grid.cpp
                    reactionProcess(i, j);
                    (stateProcess[ grid[i][j].getState() ])(j, i);
                }
        }

        for(int i = 0; i < size; ++i)
            for(int j = 0; j < size; ++j)
                grid[i][j].setProcessed(false);
    }
    void handleInput(); // defined in input.h
    void reactionProcess(int x, int y) {
        checkReaction(x, y, x+1, y);
        checkReaction(x, y, x-1, y);
        checkReaction(x, y, x, y+1);
        checkReaction(x, y, x, y-1);
    }
    // pixel grid functions 
    static bool inBounds(int x, int y) {
        return !(x<0 || y<0 || x>=size || y>=size);
    }
    static bool isEmpty(int x, int y) {
        return (grid[y][x].getElem() == ""); // empty element is ""
    }
    static std::string getElem(int x, int y) {
        return grid[y][x].getElem();
    }
    static float getDensity(int x, int y) {
        return grid[y][x].getState();
    }
    static void setPixel(int x, int y, std::string s) { // not to be confused with window.setPixel
        grid[y][x] = Pixel(s);
        render(x, y);
    }
    static void render(int x, int y) {
        image.setPixel(x, y, grid[y][x].getCol() );
    }
    static void switchPixel(int x, int y, int x2, int y2) {
        std::swap( grid[y][x], grid[y2][x2] );
        render(x,y);
        render(x2,y2);
    }
    static void checkReaction(int x, int y, int x2, int y2) {
        if( !inBounds(x2, y2) || !inBounds(x, y) ) return;
        std::string elem1 = grid[y][x].getElem();
        std::string elem2 = grid[y2][x2].getElem();
        auto it = elem::reaction[elem1].find( elem2 );
        if( elem::list[elem2].burning && elem::melt.find(elem1) != elem::melt.end() ) {
            setPixel(x, y, elem::melt[elem1] );
            return;
        }
        //auto it2 = elem::reaction[elem2].find( elem1 );
        if( it != elem::reaction[elem1].end() && randf() < elem::reaction[elem1][elem2].second )
            setPixel(x, y, elem::reaction[elem1][elem2].first);
        //if( it2 != elem::reaction[elem2].end() && elem::reaction[elem2][elem1].second ) {
        //    setPixel(x, y, elem::reaction[elem2][elem1].first );
        //}
    }
};