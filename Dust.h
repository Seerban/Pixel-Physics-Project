#include <SFML/Graphics.hpp>
#include <iostream>
#include "Pixel.h"

class Dust : public Pixel {

public:
    Dust() : Pixel() {}
    Dust(sf::Color col) : Pixel(col) {}


    void process(int x, int y) override {
        if(y > 35) return;
        if( grid[y+1][x]->getCol() == sf::Color::Black ) {
            Pixel::switch_pixel(x, y, x, y+1);
        }
    }
};