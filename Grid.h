#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include <random>
#include <utility>
#include <unordered_map>
#include "Pixel.h"

class Grid {
    int size;
    std::vector<std::vector<Pixel>> grid;
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::RenderWindow window;
public:
    static std::unordered_map<State, void(Grid::*)(int,int)> moveFunc;

    Grid() : Grid(40) {}
    Grid(int size, int scale = 8) : 
        size(size),
        window(sf::VideoMode(size * scale, size * scale), "Pixel Grid") {
            grid.resize(size, std::vector<Pixel>(size));
            image.create(size, size, sf::Color::Black);
            texture.loadFromImage(image);
            sprite.setTexture(texture);
            sprite.setScale(scale, scale);
            window.setFramerateLimit(18);
        }
    
    void start() {
        setPixel(0, 0, "water");
        setPixel(1, 0, "dirt");
        setPixel(5, 5, "sand");
        switchPixel(0,0,1,0);

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
        // bottom-to-top loop for processing liquids/dusts
        for(int i = size-1; i >= 0; --i) {
            int j, goal, incr;
            // go from left to right or right to left alternating every row
            if(i % 2 == 0) { j = 0; goal = size; incr = 1; }
            else { j = size-1; goal = 0; incr = -1; }

            while( j != goal ) {
                (this->*moveFunc[ grid[i][j].getState() ])(j, i);
                j += incr;
            }
        }
    }

    bool inBounds(int x, int y) {
        return !(x<0 || y<0 || x>=size || y>=size);
    }
    bool isEmpty(int x, int y) {
        return (grid[y][x].getElem() == ""); // empty element is ""
    }
    void setPixel(int x, int y, std::string s) { // not to be confused with window.setPixel
        grid[y][x] = Pixel(s);
        render(x, y);
    }
    void render(int x, int y) {
        image.setPixel(x, y, grid[y][x].getCol() );
        std::cout<<"rendered pixel "<<x<<' '<<y<<std::endl;
    }
    void switchPixel(int x, int y, int x2, int y2) {
        std::swap( grid[y][x], grid[y2][x2] );
        render(x,y);
        render(x2,y2);
    }

    void moveSolid(int x, int y);
    void moveDust(int x, int y);
    void moveLiquid(int x, int y);
    void moveGas(int x, int y);
};