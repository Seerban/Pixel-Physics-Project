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
    { "burning_fuel", new Liquid( "burning_fuel", sf::Color(255, 75, 85))},
    { "fuel",   new Liquid( "fuel",     sf::Color(10,  25,  75))},
    { "water",  new Liquid( "water",    sf::Color(25,  55,  235))},
    // -------------------------------- dusts --------------------------------
    { "mud",    new Dust(   "mud",      sf::Color(125, 40,  55))},
    { "sand",   new Dust(   "sand",     sf::Color(225, 225, 155))},
    // -------------------------------- gases --------------------------------
    { "steam",  new Gas(    "steam",    sf::Color(155, 155, 165))},
    { "fire",   new Gas(    "fire",     sf::Color(255, 55,  65))},
    // -------------------------------- emitters --------------------------------
    { "fire_source", new Pixel("fire_source",   sf::Color(255, 0, 0))},
    { "water_source", new Pixel("water_source", sf::Color(0, 0, 255))},
};

std::unordered_map< std::string, float> density = {
    { "fire", -2 },
    { "steam", -1 },

    { "empty", 0 },
    { "fuel", 0.5 },
    { "water", 1 },

    { "mud", 1.5 },
    { "sand", 1.5 },

    { "earth", 2 },
    
    { "fire_source", 3 },
    { "water_source", 3 },
};

// reaction, example: ["fire"]["air"] = <air, 0.5) (50% chance to disappear)
std::unordered_map< std::string, std::unordered_map<std::string, std::pair< std::string, float >>> reaction = {
    // -------------------------------- solids --------------------------------
    { "earth", {{"water",  {"mud",     1}}      }   },
    // -------------------------------- liquids --------------------------------
    { "water", {{"earth",  {"empty",   1}},
                {"fire",   {"steam",    1}},    }   },
    { "fuel", {{"fire",    {"burning_fuel", 1}}}    },
    // -------------------------------- dusts --------------------------------
    { "mud",    {{"empty",  {"earth",   0.01}}}     },
    // -------------------------------- gases --------------------------------
    { "fire",  {{"empty",  {"empty",   0.1}},
                {"water",  {"empty",   1}},     }   },
    { "steam",  {{"empty",  {"empty",   0.015}}}    },
    // -------------------------------- emitters --------------------------------
    { "empty", {{"fire_source",     {"fire",     0.75} },
                {"burning_fuel",    {"fire",     0.33}  },
                {"water_source",    {"water",    0.75} },}  },

    { "burning_fuel",{{"empty", {"empty", 0.015}}}    },
    { "fire_source", {{"empty", {"empty", 0.0025}}}   }, // bandaid fix for transformer skipping non reactive elems
    { "water_source",{{"empty", {"empty", 0.0025}}}   },
};

}