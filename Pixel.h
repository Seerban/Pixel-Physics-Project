#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include "Element.h"

const int COL_VAR = 7;
sf::Color randomize_col(sf::Color col);

class Pixel {
    std::string elem;
    sf::Color col;
    elem::State state;
    bool processed;
    float wet = 0;
public:
    // CONSTRUCTORS
    Pixel() : Pixel("") {}
    Pixel(std::string s) : elem(s), col( randomize_col(elem::list[s].col) ), state(elem::list[s].state), processed(false) {}
    
    // OPERATORS
    Pixel& operator=(const Pixel& p) {
        if (this == &p) return *this;
        elem = p.elem;
        col = p.col;
        state = p.state;
        processed = p.processed;
        wet = p.wet;
        return *this;
    }
    
    // GETTERS/SETTERS
    std::string getElem() { return this->elem; }
    void setElem(std::string s) { this->elem = s; }
    sf::Color getCol() { return this->col; }
    void setCol(sf::Color c) { this->col = c; }
    elem::State getState() { return this->state; }
    void setState(elem::State s) { this->state = s; }
    bool getProcessed() { return this->processed; }
    void setProcessed(bool b) { this->processed = b; }
};