#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <iostream>
#include "States.h"

namespace elem {

// element struct (contains only metadata, Pixel class has the dynamic data)
struct element {
    sf::Color col;
    State::State state;
    float density = 1; // reversed for gases
    bool burning = false;
    float wet = 0;
    double temperature = 30;
    bool colorful = false; // increases color variability
    bool colorless = false; // removes color variability
    bool highlight = false; // has white highlights
    bool evaporates = false; // evaporates into anothe relement
    bool fluid = false; // flows faster 
    bool sponge = false; // absorbs water
    bool flammable = false;
};

// utility function for easier defining
element el(State::State state, char* hex);

extern int intStep(int n, int m, int step);
extern void tryPlace(int x, int y, std::string element);
extern bool tryMove(int x, int y, int x2, int y2, bool gas = false);
extern int randomIncrement();

// map from state to process function
extern std::unordered_map< std::string, element > list;
extern std::unordered_map< std::string, std::unordered_map< std::string, std::pair< std::string, float > > > reaction;
extern std::unordered_map< std::string, std::string > emits;
extern std::unordered_map< std::string, std::string > hardmelt;
extern std::unordered_map< std::string, std::string > melt;
extern std::unordered_map< std::string, std::string > evap_to;
extern std::unordered_map< std::string, std::string > wet_to;
extern std::unordered_map< std::string, std::string > dry_to;
extern std::unordered_map< std::string, std::string > freeze;
extern std::unordered_map< std::string, std::string > unfreeze;
}