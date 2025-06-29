#include <SFML/Graphics.hpp>
#include "Pixel.h"
#pragma once

class Dust : public Pixel {

public:
    Dust() : Pixel() {}
    Dust(sf::Color col) : Pixel(col) {}

    void process(int x, int y) override {
        //check bleow
        if( is_empty(x, y+1) ) {
            Pixel::switch_pixel(x, y, x, y+1);
            return;
        }
        // check diagonals
        else if ( random() > 0.5 ) // choose to go left or right first
        {
            if( is_empty(x+1, y+1) ) { Pixel::switch_pixel(x, y, x+1, y+1); return; }
            if( is_empty(x-1, y+1) ) { Pixel::switch_pixel(x, y, x-1, y+1); return; }
        }
        else {
            if( is_empty(x-1, y+1) ) { Pixel::switch_pixel(x, y, x-1, y+1); return; }
            if( is_empty(x+1, y+1) ) { Pixel::switch_pixel(x, y, x+1, y+1); return; }
        }
    }
    Pixel* clone() const override { return new Dust(*this); }
};