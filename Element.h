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
    float density = 1;
    bool burning = false;
    bool evaporates = false;
    bool fluid = false;
};

// utility function for easier defining
element el(State state, char* hex);

// map from state to process function
extern std::unordered_map< std::string, element > list;
extern std::unordered_map< std::string, std::unordered_map< std::string, std::pair< std::string, float > > > reaction;
extern std::unordered_map< std::string, std::string > emits;
extern std::unordered_map< std::string, std::string > melt;
extern std::unordered_map< std::string, std::string > evap_to;
}