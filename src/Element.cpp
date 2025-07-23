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

    if( strchr(tags, 'F') ) e.flammable = true;
    if( strchr(tags, 'w') ) e.wet = 1;
    if( strchr(tags, 's') ) e.sponge = true;

    if( strchr(tags, 'c') ) e.colorful = true;
    if( strchr(tags, 'C') ) e.colorless = true;
    if( strchr(tags, 'h') ) e.highlight = true;

    if( strchr(tags, 'e') ) { e.evaporates = true; e.evap_chance = elemutil::countChar(tags, 'e') * 0.025; }
    if( strchr(tags, 'E') ) { e.evaporates = true; e.evap_chance = elemutil::countChar(tags, 'E') * 0.25; }
    if( strchr(tags, 'f') ) e.fluid = true;
    return e;
}
// List of elements
std::unordered_map< std::string, element > list {
    {"",                el(state::STATIC,   "000000", 0.5,  30,     -10000,     "",         10000,      "",         "C")},
    {"diamond",         el(state::STATIC,   "99B9F5", 3,    30,     -10000,     "",         10000,       "",       "h")},
    {"fuse",            el(state::STATIC,   "AA7E82", 3,    30,     -10000,     "",         100,      "fire_source","CF")},
    {"metal",           el(state::STATIC,   "353535", 3,    30,     -10000,     "",         1000,    "molten_metal","C")},
    {"plant",           el(state::STATIC,   "359555", 3,    30,     -10000,     "",         150,      "fire_source","")},
    {"rusted_metal",    el(state::STATIC,   "452E22", 3,    30,     -10000,     "",         1000,    "molten_metal","C")},
    {"wood",            el(state::STATIC,   "855E42", 3,    30,     -10000,     "",         100,      "fire_source","")},

    {"dirt",            el(state::SOLID,    "964B00", 2,    30,     -10000,     "",         10000,      "",         "s")},
    {"glass",           el(state::SOLID,    "DDDDFF", 2,    30,     -10000,     "",         1000,       "molten_glass","h")},
    {"ice",             el(state::SOLID,    "7788FF", 2,    -400,   -10000,     "",         0,          "water",    "h")},
    {"mud",             el(state::SOLID,    "70543E", 2,    30,     -10000,     "",         100,        "dirt",     "s")},
    {"nuke",            el(state::SOLID,    "334433", 2,    30,     -10000,     "",         100,        "explosion7","CF")},
    {"obsidian",        el(state::SOLID,    "221045", 2,    30,     -10000,     "",         600,        "lava",     "")},
    {"rock",            el(state::SOLID,    "555555", 2,    30,     -10000,     "",         10000,      "",         "")},
    {"wet_sand",        el(state::SOLID,    "A28260", 2,    30,     -10000,     "",         100,        "sand",     "s")},
    {"grass",           el(state::SOLID,    "05AA25", 2,    30,     -10000,     "",         100,        "dirt",     "")},

    
    {"burning_oil",     el(state::LIQUID,   "FF2222", 1,    1000,   600,        "",         10000,      "",         "e")},
    {"molten_glass",    el(state::LIQUID,   "DDDDFF", 2,    999,    1000,       "glass",     10000,     "",         "")},
    {"molten_metal",    el(state::LIQUID,   "653535", 2,    1000,   999,        "metal",    10000,      "",         "")},
    {"oil",             el(state::LIQUID,   "151055", 1,    30,     -10000,     "",         200,      "burning_oil","cF")},
    {"lava",            el(state::LIQUID,   "DD3505", 2,    1500,   600,        "obsidian", 10000,      "",         "")},
    {"water",           el(state::LIQUID,   "0E87CC", 1.5,  30,     0,          "ice",      100,        "steam",    "wf")},
    
    {"gravel",          el(state::DUST,     "999999", 2,    30,     -10000,     "",         10000,      "",         "h")},
    {"sand",            el(state::DUST,     "C2B280", 2,    30,     -10000,     "",         1000,       "molten_glass","smh")},
    {"grass_seed",      el(state::DUST,     "804020", 1,    30,     -10000,     "",         100,        "",         "")},

    {"cloud",           el(state::GAS,      "DDDDDD", 0.5,  30,     -10000,     "",         10000,      "",         "hCe")},
    {"fire",            el(state::GAS,      "FF5A00", 0.3,  1000,   500,        "smoke",    10000,      "",         "h")},
    {"plasma",          el(state::GAS,      "C321A5", 0.4,  2500,   1000,       "smoke",    10000,      "",         "h")},
    {"smoke",           el(state::GAS,      "333333", 0.2,  30,     -10000,     "",         10000,      "",         "e")},
    {"steam",           el(state::GAS,      "888888", 0.1,  30,     -10000,     "",         10000,      "",         "e")},

    {"oxygen",          el(state::NEUTRAL,  "5566DD", 0.5,  30,     -10000,     "",         200,        "fire",     "F")},
    {"spark",           el(state::NEUTRAL,  "FFFF22", 0.5,  2000,   -10000,     "",         10000,      "",         "E")},

    {"fire_source",     el(state::EMITTER,  "FF5A00", 10,   1000,   500,        "smoke",    10000,      "",         "eee")},
    {"water_source",    el(state::EMITTER,  "0E87CC", 10,   30,     0,          "ice",      100,        "steam",    "")},
    {"spark_source",    el(state::EMITTER,  "FFFF22", 10,   2000,   -10000,     "",         10000,      "",         "hEEE")},

    {"bug",             el(state::LIFEFORM, "152535", 0.8,  30,     0,          "",         80,         "",         "c")},

    {"explosion",       el(state::EXPLOSION,"FFFFFF", 0.5,  2000,   -10000,     "",         10000,      "",         "")},
    {"explosion4",      el(state::EXPLOSION,"FFFFFF", 0.5,  2000,   -10000,     "",         10000,      "",         "")},
    {"explosion7",      el(state::EXPLOSION,"FFFFFF", 0.5,  3000,   -10000,     "",         10000,      "",         "")},
};
// Element Reactions
std::unordered_map< std::string, std::unordered_map< std::string, std::string> > reaction {
    {"dirt", { {"grass_seed", "grass"}, {"explosion", ""}}},
    {"fuse", { {"fire_source", "fire_source"} }},
    {"grass", { {"bug", "bug"}, {"explosion", ""}}},
    {"grass_seed", { {"dirt", ""} }},
    {"metal",   { {"water", "rusted_metal"} }},
    {"rock",    { {"explosion", "gravel"} }},
    {"water",   { {"plant", "plant"} }},
    {"plant",   { {"bug", "bug"} }},
};
// Element turns air to elem
std::unordered_map< std::string, std::string > emits {
    {"fire_source",     "fire"},
    {"water_source",    "water"},
    {"spark_source",    "spark"},
};

std::unordered_map< std::string, std::string > evap_to {
    {"cloud", "water"},
};
// if it has high humidity
std::unordered_map< std::string, std::string > wet_to {
    {"dirt", "mud"},
    {"sand", "wet_sand"},
};
// Movement process for each state
// returns 1 if erased/transformed
}
