#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "States.h"
#include "Element.h"

class Pixel {
    std::string elem;
    sf::Color col;
    State state;
    bool processed;
public:
    // CONSTRUCTORS
    Pixel() : Pixel("") {}
    Pixel(std::string s) : elem(s), col(elem::list[s].col), state(elem::list[s].state), processed(false) {}
    
    // OPERATORS
    Pixel& operator=(const Pixel& p) {
        if (this == &p) return *this;
        elem = p.elem;
        col = p.col;
        state = p.state;
        processed = p.processed;
        return *this;
    }
    
    // GETTERS/SETTERS
    std::string getElem() { return this->elem; }
    void setElem(std::string s) { this->elem = s; }
    sf::Color getCol() { return this->col; }
    void setCol(sf::Color c) { this->col = c; }
    State getState() { return this->state; }
    void setState(State s) { this->state = s; }
    bool getProcessed() { return this->processed; }
    void setProcessed(bool b) { this->processed = b; }
};