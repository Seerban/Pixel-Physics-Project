#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <vector>
#include <iostream>
#include <string>
#include "States.h"

namespace elem {

    enum Elements {
        VOID,
        DIAMOND, FUSE, METAL, PLANT, RUSTED_METAL, WOOD,
        DIRT, GLASS, ICE, MUD, NUKE, OBSIDIAN, ROCK, WET_SAND, GRASS,
        BURNING_OIL, MOLTEN_GLASS, MOLTEN_METAL, OIL, LAVA, WATER,
        GRAVEL, SAND, GRASS_SEED,
        CLOUD, FIRE, PLASMA, SMOKE, STEAM,
        OXYGEN, SPARK,
        FIRE_SOURCE, WATER_SOURCE, SPARK_SOURCE,
        BUG,
        EXPLOSION, EXPLOSION4, EXPLOSION7,
    };
// element struct (contains only metadata, Pixel class has the dynamic data)
    struct element {
        std::string name;
        sf::Color col;
        state::States elemstate;
        float density = 1; // reversed for gases
        bool burning = false;
        float wet = 0;
        double temperature = 30;
        int freeze_temp = 0;
        Elements freeze_to;
        int melt_temp = 0;
        Elements melt_to;
        bool colorful = false; // increases color variability
        bool colorless = false; // removes color variability
        bool highlight = false; // has white highlights
        bool evaporates = false; // evaporates into anothe relement
        float evap_chance = 0;
        bool fluid = false; // flows faster 
        bool sponge = false; // absorbs water
        bool flammable = false;
    };

    element el(std::string name, state::States elemstate, const char* hex, float density, int temperature, int freezing_point, Elements freeze_to, int melting_point, Elements melt_to, const char* tags);

    // Element list
    extern std::vector< element > list;

    extern void init_list();
    // Element reactions
    extern std::unordered_map< Elements, std::unordered_map< Elements, Elements> >reaction;
    extern std::unordered_map< Elements, Elements > emits;
    extern std::unordered_map< Elements, Elements > evap_to;
    extern std::unordered_map< Elements, Elements > wet_to;
}