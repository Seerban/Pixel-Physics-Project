#pragma once
#include <SFML/Graphics.hpp>
#include "Pixel.h"

class Liquid : public Pixel {
    int horizontal_seeking = 2;
    int hseek_it = 0;
public:
    Liquid() : Pixel() {}
    Liquid(std::string elem, sf::Color col) : Pixel(elem, col) {}

    void horSeek(int x, int y) {
        if(hseek_it > 0) {
            --hseek_it;
            movement(x, y);
        } else
            hseek_it = horizontal_seeking;
    }

    void movement(int x, int y) override {
        if( tryMove(x, y, x, y+1) ) return;
        else if ( random() > 0.5 ) // choose to go left or right first
        {
            if( tryMove(x, y, x+1, y+1) ) return;
            if( tryMove(x, y, x-1, y+1) ) return;
        }
        else {
            if( tryMove(x, y, x-1, y+1) ) return;
            if( tryMove(x, y, x+1, y+1) ) return;
        }
        // go horizontally 
        if ( random() > 0.5 ) // choose to go left or right first
        {
            if( tryMove(x, y, x+1, y) ) { horSeek(x+1, y ); return; }
            if( tryMove(x, y, x-1, y) ) { horSeek(x-1, y); return; }
        }
        else {
            if( tryMove(x, y, x-1, y) ) { horSeek(x-1, y); return; }
            if( tryMove(x, y, x+1, y) ) { horSeek(x+1, y); return; }
        }
    }

    Pixel* clone() const override { return new Liquid(*this); }
};