#include "Element.h"
#include "Grid.h"
#include "States.h"
#include <cstdlib>
#include <cstring>

// contains list of elements

namespace elem {

element el(state::States elemstate, const char* hex, float density, int temperature, int freezing_point, std::string freeze_to, int melting_point, std::string melt_to, const char* tags) {
    element e;
    int r, g, b;
    sscanf(hex, "%02x%02x%02x", &r, &g, &b);
    e.col = sf::Color(r,g,b);
    e.elemstate = elemstate;
    e.density = density;
    e.temperature = temperature;
    e.freeze_temp = freezing_point;
    e.freeze_to = freeze_to;
    e.melt_temp = melting_point;
    e.melt_to = melt_to;

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
    {"",                el(state::SOLID,    "000000", 0.5,  30,     -10000,     "",         10000,      "",         "C")},
    {"dirt",            el(state::SOLID,    "964B00", 2,    30,     -10000,     "",         10000,      "",         "s")},
    {"glass",           el(state::SOLID,    "DDDDFF", 2,    30,     -10000,     "",         10000,      "",         "h")},
    {"ice",             el(state::SOLID,    "7788FF", 2,    -400,   -10000,     "",         0,          "water",    "h")},
    {"mud",             el(state::SOLID,    "70543E", 2,    30,     -10000,     "",         100,        "dirt",     "s")},
    {"obsidian",        el(state::SOLID,    "221045", 2,    30,     -10000,     "",         600,        "lava",     "")},
    {"rock",            el(state::SOLID,    "555555", 2,    30,     -10000,     "",         10000,      "",         "")},
    {"wet_sand",        el(state::SOLID,    "A28260", 2,    30,     -10000,     "",         100,        "sand",     "s")},
    {"grass",           el(state::SOLID,    "05AA25", 2,    30,     -10000,     "",         100,        "dirt",     "")},
    
    {"burning_oil",     el(state::LIQUID,   "FF2222", 1,    1000,   600,        "",         10000,      "",         "")},
    {"oil",             el(state::LIQUID,   "151055", 1,    30,     -10000,     "",         10000,      "",         "cE")},
    {"lava",            el(state::LIQUID,   "DD3505", 2,    1500,   600,        "obsidian", 10000,      "",         "")},
    {"water",           el(state::LIQUID,   "0E87CC", 1.5,  30,     0,          "ice",      100,        "steam",    "wf")},
    
    {"gravel",          el(state::DUST,     "999999", 2,    30,     -10000,     "",         10000,      "",         "h")},
    {"sand",            el(state::DUST,     "C2B280", 2,    30,     -10000,     "",         1000,       "glass",    "smh")},
    {"grass_seed",      el(state::DUST,     "804020", 1,    30,     -10000,     "",         100,        "",         "")},

    {"fire",            el(state::GAS,      "FF5A00", 0.3,  750,    500,        "smoke",    10000,      "",         "")},
    {"plasma",          el(state::GAS,      "C321A5", 0.4,  2500,   1000,       "",         10000,      "",         "h")},
    {"smoke",           el(state::GAS,      "333333", 0.2,  30,     -10000,     "",         10000,      "",         "e")},
    {"steam",           el(state::GAS,      "888888", 0.1,  30,     -10000,     "",         10000,      "",         "e")},

    {"fire_source",     el(state::EMITTER,  "FF5A00", 10,   750,    500,        "smoke",    10000,      "",         "")},
    {"water_source",    el(state::EMITTER,  "0E87CC", 10,   30,     0,          "ice",      100,        "steam",    "")},

    {"bug",             el(state::LIFEFORM, "152535", 0.8,  30,     0,          "",         80,         "",         "c")},
};
// Element Reactions
std::unordered_map< std::string, std::unordered_map< std::string, std::string> > reaction {
    {"grass_seed", { {"dirt", ""} }},
    {"dirt", { {"grass_seed", "grass"} }},
    {"grass", { {"bug", "bug"} }},
};
// Element turns air to elem
std::unordered_map< std::string, std::string > emits {
    {"fire_source",     "fire"},
    {"water_source",    "water"},
};

std::unordered_map< std::string, std::string > evap_to {
};
// if it has high humidity
std::unordered_map< std::string, std::string > wet_to {
    {"dirt", "mud"},
    {"sand", "wet_sand"},
};
// Movement process for each state
// returns 1 if erased/transformed
}
