#pragma once
#include <SFML/Graphics.hpp>
#include "Pixel.h"

class Liquid : public Pixel {

public:
    Liquid() : Pixel() {}
    Liquid(std::string elem, sf::Color col) : Pixel(elem, col) {}

    void movement(int x, int y) override {
        //check bleow
        if( inBounds(x, y+1) && is_empty(x, y+1) ) {
            Pixel::switch_pixel(x, y, x, y+1);
            return;
        }
        // check diagonals
        else if ( random() > 0.5 ) // choose to go left or right first
        {
            if( inBounds(x+1, y+1) && is_empty(x+1, y+1) ) { Pixel::switch_pixel(x, y, x+1, y+1); return; }
            if( inBounds(x-1, y+1) && is_empty(x-1, y+1) ) { Pixel::switch_pixel(x, y, x-1, y+1); return; }
        }
        else {
            if( inBounds(x-1, y+1) && is_empty(x-1, y+1) ) { Pixel::switch_pixel(x, y, x-1, y+1); return; }
            if( inBounds(x+1, y+1) && is_empty(x+1, y+1) ) { Pixel::switch_pixel(x, y, x+1, y+1); return; }
        }
        // go horizontally 
        if ( random() > 0.5 ) // choose to go left or right first
        {
            if( inBounds(x+1, y) && is_empty(x+1, y) ) { Pixel::switch_pixel(x, y, x+1, y); return; }
            if( inBounds(x-1, y) && is_empty(x-1, y) ) { Pixel::switch_pixel(x, y, x-1, y); return; }
        }
        else {
            if( inBounds(x-1, y) && is_empty(x-1, y) ) { Pixel::switch_pixel(x, y, x-1, y); return; }
            if( inBounds(x+1, y) && is_empty(x+1, y) ) { Pixel::switch_pixel(x, y, x+1, y); return; }
        }
    }
    Pixel* clone() const override { return new Liquid(*this); }
};