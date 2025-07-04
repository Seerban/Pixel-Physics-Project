#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "States.h"

namespace elem {

struct element {
    sf::Color col;
    State state;
};

// utility function for easier defining
element el(State state, char* hex);

extern std::unordered_map< std::string, element > list;

}