#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>
#include "States.h"

namespace elem {

// element struct (contains only metadata, Pixel class has the dynamic data)
struct element {
    sf::Color col;
    state::States elemstate;
    float density = 1; // reversed for gases
    bool burning = false;
    float wet = 0;
    double temperature = 30;
    int freeze_temp = 0;
    std::string freeze_to;
    int melt_temp = 0;
    std::string melt_to;
    bool colorful = false; // increases color variability
    bool colorless = false; // removes color variability
    bool highlight = false; // has white highlights
    bool evaporates = false; // evaporates into anothe relement
    float evap_chance = 0;
    bool fluid = false; // flows faster 
    bool sponge = false; // absorbs water
    bool flammable = false;
};

element el(state::States elemstate, const char* hex, float density, int temperature, int freezing_point, std::string freeze_to, int melting_point, std::string melt_to, const char* tags);

// Element list
extern std::unordered_map< std::string, element > list;

// Element reactions
extern std::unordered_map< std::string, std::unordered_map< std::string, std::string> > reaction;
extern std::unordered_map< std::string, std::string > emits;
extern std::unordered_map< std::string, std::string > evap_to;
extern std::unordered_map< std::string, std::string > wet_to;
}