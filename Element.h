#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <iostream>

namespace elem {
// element states of matter (ordered by default density, can be modified by element)
enum State {
    GAS,
    LIQUID, 
    DUST, 
    SOLID, 
    EMITTER, // "spawns" other pixels and is SOLID
};

// state processes
void solidProcess(int x, int y);
void dustProcess(int x, int y);
void liquidProcess(int x, int y);
void gasProcess(int x, int y);
void emitterProcess(int x, int y);

// element struct (contains only metadata, Pixel class has the dynamic data)
struct element {
    sf::Color col;
    State state;
    float density = 1; // reversed for gases
    bool burning = false;
    double temperature = 30;
    bool colorful = false; // increases color variability
    bool colorless = false; // removes color variability
    bool highlight = false; // has white highlights
    bool evaporates = false; // evaporates into anothe relement
    bool fluid = false; // flows faster 
    bool sponge = false; // absorbs water
    bool wet = false; // starts with 1 humidty
    bool flammable = false;
};

// utility function for easier defining
element el(State state, char* hex);

extern int intStep(int n, int m, int step);

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