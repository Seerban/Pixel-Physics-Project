#include "Element.h"
#include "Grid.h"
#include "States.h"
#include <cstdlib>
#include <cstring>

// contains list of elements

namespace elem {

element el(std::string name, state::States elemstate, const char* hex, float density, int temperature, int freezing_point, Elements freeze_to, int melting_point, Elements melt_to, const char* tags) {
    element e;
    e.name = name;
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
std::vector< element >  list;

void init_list() {
    list.resize(100);

    list[VOID] =        el("Void",          state::STATIC,   "000000", 0.5,  30,     -10000, VOID,         10000, VOID,         "C");
    list[DIAMOND] =     el("Diamond",       state::STATIC,   "99B9F5", 3,    30,     -10000, VOID,         10000, VOID,         "h");
    list[FUSE] =        el("Fuse",          state::STATIC,   "AA7E82", 3,    30,     -10000, VOID,         100,   FIRE_SOURCE,   "CF");
    list[METAL] =       el("Metal",         state::STATIC,   "353535", 3,    30,     -10000, VOID,         1000,  MOLTEN_METAL,  "C");
    list[PLANT] =       el("Plant",         state::STATIC,   "359555", 3,    30,     -10000, VOID,         150,   FIRE_SOURCE,   "");
    list[RUSTED_METAL] =el("Rusted Metal",  state::STATIC,   "452E22", 3,    30,     -10000, VOID,         1000,  MOLTEN_METAL,  "C");
    list[WOOD] =        el("Wood",          state::STATIC,   "855E42", 3,    30,     -10000, VOID,         100,   FIRE_SOURCE,   "");

    list[DIRT] =        el("Dirt",          state::SOLID,    "964B00", 2,    30,     -10000, VOID,         10000, VOID,         "s");
    list[GLASS] =       el("Glass",         state::SOLID,    "DDDDFF", 2,    30,     -10000, VOID,         1000,  MOLTEN_GLASS,  "h");
    list[ICE] =         el("Ice",           state::SOLID,    "7788FF", 2,   -400,    -10000, VOID,         0,     WATER,         "h");
    list[MUD] =         el("Mud",           state::SOLID,    "70543E", 2,    30,     -10000, VOID,         100,   DIRT,          "s");
    list[NUKE] =        el("Nuke",          state::SOLID,    "334433", 2,    30,     -10000, VOID,         100,   EXPLOSION7,    "CF");
    list[OBSIDIAN] =    el("Obsidian",      state::SOLID,    "221045", 2,    30,     -10000, VOID,         600,   LAVA,          "");
    list[ROCK] =        el("Rock",          state::SOLID,    "555555", 2,    30,     -10000, VOID,         10000, VOID,         "");
    list[WET_SAND] =    el("Wet Sand",      state::SOLID,    "A28260", 2,    30,     -10000, VOID,         100,   SAND,          "s");
    list[GRASS] =       el("Grass",         state::SOLID,    "05AA25", 2,    30,     -10000, VOID,         100,   DIRT,          "");

    list[BURNING_OIL] = el("Burning Oil",   state::LIQUID,   "FF2222", 1,   1000,     600,    VOID,         10000, VOID,         "e");
    list[MOLTEN_GLASS] =el("Molten Glass",  state::LIQUID,   "DDDDFF", 2,    999,    1000,    GLASS,        10000, VOID,         "");
    list[MOLTEN_METAL] =el("Molten Metal",  state::LIQUID,   "653535", 2,   1000,     999,    METAL,        10000, VOID,         "");
    list[OIL] =         el("Oil",           state::LIQUID,   "151055", 1,     30,   -10000,    VOID,         200,   BURNING_OIL,  "cF");
    list[LAVA] =        el("Lava",          state::LIQUID,   "DD3505", 2,   1500,     600,    OBSIDIAN,     10000, VOID,         "");
    list[WATER] =       el("Water",         state::LIQUID,   "0E87CC", 1.5,   30,       0,    ICE,          100,   STEAM,        "wf");

    list[GRAVEL] =      el("Gravel",        state::DUST,     "999999", 2,    30,     -10000, VOID,         10000, VOID,         "h");
    list[SAND] =        el("Sand",          state::DUST,     "C2B280", 2,    30,     -10000, VOID,         1000,  MOLTEN_GLASS,  "smh");
    list[GRASS_SEED] =  el("Grass Seed",    state::DUST,     "804020", 1,    30,     -10000, VOID,         100,   VOID,         "");

    list[CLOUD] =       el("Cloud",         state::GAS,      "DDDDDD", 0.5,  30,     -10000, VOID,         10000, VOID,         "hCe");
    list[FIRE] =        el("Fire",          state::GAS,      "FF5A00", 0.3, 1000,     500,    SMOKE,        10000, VOID,         "h");
    list[PLASMA] =      el("Plasma",        state::GAS,      "C321A5", 0.4, 2500,    1000,    SMOKE,        10000, VOID,         "h");
    list[SMOKE] =       el("Smoke",         state::GAS,      "333333", 0.2,  30,     -10000, VOID,         10000, VOID,         "e");
    list[STEAM] =       el("Steam",         state::GAS,      "888888", 0.1,  30,     -10000, VOID,         10000, VOID,         "e");

    list[OXYGEN] =      el("Oxygen",        state::NEUTRAL,  "5566DD", 0.5,  30,     -10000, VOID,         200,   FIRE,         "F");
    list[SPARK] =       el("Spark",         state::NEUTRAL,  "FFFF22", 0.5, 2000,    -10000, VOID,         10000, VOID,         "E");

    list[FIRE_SOURCE] = el("Fire Source",   state::EMITTER,  "FF5A00", 10,  1000,     500,    SMOKE,        10000, VOID,         "eee");
    list[WATER_SOURCE] =el("Water Source",  state::EMITTER,  "0E87CC", 10,    30,       0,    ICE,          100,   STEAM,        "");
    list[SPARK_SOURCE] =el("Spark Source",  state::EMITTER,  "FFFF22", 10,  2000,    -10000, VOID,         10000, VOID,         "hEEE");

    list[BUG] =         el("Bug",           state::LIFEFORM, "152535", 0.8,   30,       0,    VOID,         80,    VOID,         "c");

    list[EXPLOSION] =   el("Explosion",     state::EXPLOSION,"FFFFFF", 0.5, 2000,    -10000, VOID,         10000, VOID,         "");
    list[EXPLOSION4] =  el("Explosion4",    state::EXPLOSION,"FFFFFF", 0.5, 2000,    -10000, VOID,         10000, VOID,         "");
    list[EXPLOSION7] =  el("Explosion7",    state::EXPLOSION,"FFFFFF", 0.5, 3000,    -10000, VOID,         10000, VOID,         "");
}


// Element Reactions
std::unordered_map< Elements, std::unordered_map< Elements, Elements> > reaction {
    {DIRT, { {GRASS_SEED, GRASS}, {EXPLOSION, VOID} }},
    {FUSE, { {FIRE_SOURCE, FIRE_SOURCE} }},
    {GRASS, { {BUG, BUG}, {EXPLOSION, VOID} }},
    {GRASS_SEED, { {DIRT, VOID } }},
    {METAL,   { {WATER, RUSTED_METAL} }},
    {ROCK,    { {EXPLOSION, GRAVEL} }},
    {WATER,   { {PLANT, PLANT} }},
    {PLANT,   { {BUG, BUG} }},
};
// Element turns air to elem
std::unordered_map< Elements, Elements > emits {
    {FIRE_SOURCE,     FIRE},
    {WATER_SOURCE,    WATER},
    {SPARK_SOURCE,    SPARK},
};

std::unordered_map< Elements, Elements > evap_to {
    {CLOUD, WATER},
};
// if it has high humidity
std::unordered_map< Elements, Elements > wet_to {
    {DIRT, MUD},
    {SAND, SAND},
};
}
