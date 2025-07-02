#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include "Pixel.h"
#include "Dust.h"
#include "Gas.h"
#include "Liquid.h"
#include "Elements.h"

namespace elems {

std::unordered_map< std::string, Pixel* > elements = {
    { "empty",  new Pixel(  "empty",    sf::Color(25, 25, 25))},
    // -------------------------------- solids --------------------------------
    { "earth",  new Pixel(  "earth",    sf::Color(155, 55,  35))},
    // -------------------------------- liquids --------------------------------
    { "water",  new Liquid( "water",    sf::Color(25,  55,  235))},
    // -------------------------------- dusts --------------------------------
    { "mud",    new Dust(   "mud",      sf::Color(125, 40,  55))},
    { "sand",   new Dust(   "sand",     sf::Color(225, 225, 155))},
    // -------------------------------- gases --------------------------------
    { "steam",  new Gas(    "steam",    sf::Color(155, 155, 165))},
    { "fire",   new Gas(    "fire",     sf::Color(255, 55,  65))},
    // -------------------------------- emitters --------------------------------
    { "fire_source", new Pixel("fire_source",   sf::Color(255, 0, 0))},
};

// reaction, example: ["fire"]["air"] = <air, 0.5) (50% chance to disappear)
std::unordered_map< std::string, std::unordered_map<std::string, std::pair< std::string, float >>> reaction = {
    // -------------------------------- solids --------------------------------
    { "earth", {{"water", {"mud",   1}}}    },
    // -------------------------------- liquids --------------------------------
    { "water", {{"earth", {"empty", 1}}}    },
    // -------------------------------- dusts --------------------------------
    // -------------------------------- gases --------------------------------
    { "fire", {{"empty", {"empty", 0.1}}}   },
    // -------------------------------- emitters --------------------------------
    { "empty", {{"fire_source", {"fire", 0.75}}}    },
    { "fire_source", {{"empty", {"empty", 0.01}}} }, // bandaid fix for transformer skipping non reactive elems
};

}