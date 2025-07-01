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
    { "water",  new Liquid( "water",    sf::Color(25,   55,     235))},
    { "earth",  new Pixel(  "earth",    sf::Color(155,  55,     35))},
    { "mud",    new Dust(   "mud",      sf::Color(125,  40,     55))},
    { "steam",  new Gas(    "steam",    sf::Color(155,  155,    165))},
    { "sand",   new Dust(   "sand",     sf::Color(225,  225,    155))}
};

std::unordered_map< std::string, std::unordered_map<std::string, std::string>> reaction = {
    { "water", { {"earth", "mud"} } },
    { "earth", { {"water", "mud"} } }
};

}