#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include "Pixel.h"
#pragma once

class Dust : public Pixel {

public:
    Dust() : Pixel() {}
    Dust(sf::Color col) : Pixel(col) {}


    void process(int x, int y) override {
        //if(y > 35) return;
        if( x == grid.size()-1 || y == grid.size()-1 ) return;
        if( is_empty(x, y+1) ) Pixel::switch_pixel(x, y, x, y+1);
        else if ( random() > 0.5 ) // choose to go left or right first
        {
            if( is_empty(x+1, y+1) ) Pixel::switch_pixel(x, y, x+1, y+1);
            if( is_empty(x-1, y+1) ) Pixel::switch_pixel(x, y, x-1, y+1);
        }
        else {
            if( is_empty(x-1, y+1) ) Pixel::switch_pixel(x, y, x-1, y+1);
            if( is_empty(x+1, y+1) ) Pixel::switch_pixel(x, y, x+1, y+1);
        }

    }
};