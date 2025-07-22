#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include "Element.h"

class Pixel {
    std::string elem;
    sf::Color col;
    state::States state;
    bool processed;
    float wet = 0; // humidity
    double temp = 30; // assumed room temperature
public:
    // CONSTRUCTORS
    Pixel() : Pixel("") {}
    Pixel(std::string s) : elem(s), col( randomize_col(elem::list[s].col) ), state(elem::list[s].elemstate), processed(false) {}
    
    // OPERATORS
    Pixel& operator=(const Pixel& p) {
        if (this == &p) return *this;
        elem = p.elem;
        col = p.col;
        state = p.state;
        processed = p.processed;
        wet = p.wet;
        temp = p.temp;
        return *this;
    }
    
    // GETTERS/SETTERS
    std::string getElem() { return this->elem; }
    void setElem(std::string s) { this->elem = s; }
    sf::Color getCol() { return this->col; }
    void setCol(sf::Color c) { this->col = c; }
    state::States getState() { return this->state; }
    void setState(state::States s) { this->state = s; }
    bool getProcessed() { return this->processed; }
    void setProcessed(bool b) { this->processed = b; }
    float getWet() { return this->wet; }
    void setWet(float f) { this->wet = f; }
    double getTemp() { return this->temp; }
    void setTemp(double t) { this->temp = t; }
    // Color randomizer
    sf::Color randomize_col(sf::Color col) {
        // random col variation
        int col_var = 6;
        if( elem::list[ getElem() ].colorful ) col_var = 20;
        if( elem::list[ getElem() ].colorless ) col_var = 1;
        int dr = ( rand() % (col_var*2+1) ) - col_var;
        int dg = ( rand() % (col_var*2+1) ) - col_var;
        int db = ( rand() % (col_var*2+1) ) - col_var;

        if( elem::list[ getElem() ].highlight && rand() % 4 == 0 ) {
            int high = rand() % col_var * 6;
            dr += high;
            dg += high;
            db += high;
        }
        int r = std::clamp(int(col.r) + dr, 0, 255);
        int g = std::clamp(int(col.g) + dg, 0, 255);
        int b = std::clamp(int(col.b) + db, 0, 255);

        return sf::Color(r, g, b);
    }
};