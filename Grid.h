#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include <random>
#include <utility>
#include "Pixel.h"

class Grid {
    static bool even_tick; // alternates every process tick
    static int size; // size of grid on both axis
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
        setPixel(20, 20, "fire_source");
        setPixel(20, 35, "water_source");

        while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            }

        mainProcess();
        texture.update(image);

        window.clear();
        window.draw(sprite);
        window.display();
        }
    }
    void mainProcess() {
        even_tick = !even_tick;
        // bottom-to-top loop for processing liquids/dusts
        for(int i = size-1; i >= 0; --i) {
            int j, goal, incr;
            // go from left to right or right to left alternating every row
            if( (i + int(even_tick)) % 2  == 0) { j = 0; goal = size; incr = 1; }
            else { j = size-1; goal = 0; incr = -1; }

            while( j != goal ) {
                if( grid[i][j].getState() != elem::GAS && !grid[i][j].getProcessed() ) {
                    grid[i][j].setProcessed(true);
                    // movement utility functions defined in grid.cpp
                    (stateProcess[ grid[i][j].getState() ])(j, i);
                }
                j += incr;
            }
        }
        // top-to-bottom loop for processing gas
        for(int i = 0; i < size; ++i) {
            int j, goal, incr;
            // go from left to right or right to left alternating every row
            if( (i + int(even_tick)) % 2  == 0) { j = 0; goal = size; incr = 1; }
            else { j = size-1; goal = 0; incr = -1; }

            while( j != goal ) {
                if( grid[i][j].getState() == elem::GAS && !grid[i][j].getProcessed() ) {
                    grid[i][j].setProcessed(true);
                    // movement utility functions defined in grid.cpp
                    (stateProcess[ grid[i][j].getState() ])(j, i);
                }
                j += incr;
            }
        }

        for(int i = 0; i < size; ++i)
            for(int j = 0; j < size; ++j)
                grid[i][j].setProcessed(false);
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
};