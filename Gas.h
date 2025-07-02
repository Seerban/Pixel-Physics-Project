#pragma once
#include <SFML/Graphics.hpp>
#include "Pixel.h"

class Gas : public Pixel {

public:
    Gas() : Pixel() {}
    Gas(std::string elem, sf::Color col) : Pixel(elem, col) {}

    void movement(int x, int y) override {
        if( random() > 0.25 && tryMove(x, y, x, y-1)) return;
        else if ( random() > 0.5 ) // choose to go left or right first
        {
            if( tryMove(x, y, x+1, y-1) ) return;
            if( tryMove(x, y, x-1, y-1) ) return;
        }
        else {
            if( tryMove(x, y, x-1, y-1) ) return;
            if( tryMove(x, y, x+1, y-1) ) return;
        }
        // go horizontally 
        if ( random() > 0.5 ) // choose to go left or right first
        {
            if( tryMove(x, y, x+1, y) ) return;
            if( tryMove(x, y, x-1, y) ) return;
        }
        else {
            if( tryMove(x, y, x-1, y) ) return;
            if( tryMove(x, y, x+1, y) ) return;
        }
    }

    Pixel* clone() const override { return new Gas(*this); }
};