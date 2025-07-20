#include "Element.h"
#include "Grid.h"
#include "States.h"
#include <cstdlib>
#include <cstring>

// contains list of elements

namespace elem {

element el(state::States elemstate, const char* hex, float density, const char* tags) {
    element e;
    int r, g, b;
    sscanf(hex, "%02x%02x%02x", &r, &g, &b);
    e.col = sf::Color(r,g,b);
    e.elemstate = elemstate;
    e.density = density;
    if( strchr(tags, 'b') ) { e.burning = true; e.temperature += 750; }
    if( strchr(tags, 'B') ) { e.burning = true; e.temperature += 1500; }
    if( strchr(tags, 'F') ) e.temperature -= 300;
    if( strchr(tags, 'E') ) e.flammable = true;
    if( strchr(tags, 'w') ) e.wet = 1;
    if( strchr(tags, 's') ) e.sponge = true;

    if( strchr(tags, 'c') ) e.colorful = true;
    if( strchr(tags, 'C') ) e.colorless = true;
    if( strchr(tags, 'h') ) e.highlight = true;

    if( strchr(tags, 'e') ) e.evaporates = true;
    if( strchr(tags, 'f') ) e.fluid = true;
    return e;
}
// List of elements
std::unordered_map< std::string, element > list {
    {"",                el(state::SOLID,   "000000", 0.5,  "C")},
    {"dirt",            el(state::SOLID,   "964B00", 2,    "s")},
    {"glass",           el(state::SOLID,   "DDDDFF", 2,    "h")},
    {"ice",             el(state::SOLID,   "7788FF", 2,    "hF")},
    {"mud",             el(state::SOLID,   "70543E", 2,    "s")},
    {"obsidian",        el(state::SOLID,   "221045", 2)},
    {"rock",            el(state::SOLID,   "555555", 2)},
    {"wet_sand",        el(state::SOLID,   "A28260", 2,    "s")},

    {"burning_gasoline",el(state::LIQUID,  "FF2222", 0.9,  "b")},
    {"gasoline",        el(state::LIQUID,  "151055", 0.9,  "cE")},
    {"lava",            el(state::LIQUID,  "DD3505", 2,    "B")},
    {"water",           el(state::LIQUID,  "0E87CC", 1,    "wf")},

    {"gravel",          el(state::DUST,    "999999", 2,    "h")},
    {"sand",            el(state::DUST,    "C2B280", 2,    "smh")},

    {"fire",            el(state::GAS,     "FF5A00", 0.3,  "b")},
    {"plasma",          el(state::GAS,     "C321A5", 0.4,  "Bbch")},
    {"smoke",           el(state::GAS,     "333333", 0.2,  "e")},
    {"steam",           el(state::GAS,     "888888", 0.1,  "e")},

    {"fire_source",     el(state::EMITTER, "FF5A00", 10,   "b")},
    {"water_source",    el(state::EMITTER, "0E87CC", 10)},
};
// Element Reactions
std::unordered_map< std::string, std::unordered_map< std::string, std::pair< std::string, float > > > reaction {
};
// Element turns air to elem
std::unordered_map< std::string, std::string > emits {
    {"fire_source",     "fire"},
    {"water_source",    "water"},
};
// boiling temperature
std::unordered_map< std::string, std::string > melt {
    {"gasoline",    "burning_gasoline"},
    {"sand",    "glass"},
    {"water",   "steam"},
};
// higher temperature melt
std::unordered_map< std::string, std::string > hardmelt {
    {"obsidian", "lava"},
};
// if it's not in list, it will disappear
std::unordered_map< std::string, std::string > evap_to {
    
};
// if it has high humidity
std::unordered_map< std::string, std::string > wet_to {
    {"dirt", "mud"},
    {"sand", "wet_sand"},
};
// if high temp will lose water
std::unordered_map< std::string, std::string > dry_to {
    {"water", ""}, // dries in wet process, others dry in temp process
    {"mud", "dirt"},
    {"wet_sand", "sand"},
};
// below 0 temp
// when burning process stops
std::unordered_map< std::string, std::string > freeze {
    {"lava", "obsidian"},
    {"fire", "smoke"},
    {"plasma", "smoke"},
    {"water", "ice"},
};
// above 0 temp
std::unordered_map< std::string, std::string > unfreeze {
    {"ice", "water"},
};

// Movement process for each state
// returns 1 if erased/transformed
}
