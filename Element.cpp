#include "Element.h"
#include "Grid.h"
#include <cstdlib>
#include <cstring>

// contains list of elements

namespace elem {

element el(State state, const char* hex, float density = 1, const char* tags = "") {
    element e;
    int r, g, b;
    sscanf(hex, "%02x%02x%02x", &r, &g, &b);
    e.col = sf::Color(r,g,b);
    e.state = state;
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
    {"",                el(SOLID,   "000000", 0.5)},
    {"dirt",            el(SOLID,   "964B00", 2,    "s")},
    {"glass",           el(SOLID,   "DDDDFF", 2,    "h")},
    {"ice",             el(SOLID,   "7788FF", 2,    "hF")},
    {"mud",             el(SOLID,   "70543E", 2,    "sw")},
    {"obsidian",        el(SOLID,   "221045", 2)},
    {"rock",            el(SOLID,   "555555", 2)},
    {"wet_sand",        el(SOLID,   "A28260", 2,    "s")},

    {"burning_gasoline",el(LIQUID,  "FF2222", 0.9,  "b")},
    {"gasoline",        el(LIQUID,  "1510555", 0.9, "cE")},
    {"lava",            el(LIQUID,  "DD3505", 2,    "B")},
    {"water",           el(LIQUID,  "0E87CC", 1,    "wf")},

    {"gravel",          el(DUST,    "999999", 2,    "h")},
    {"sand",            el(DUST,    "C2B280", 2,    "smh")},

    {"fire",            el(GAS,     "FF5A00", 0.3,  "b")},
    {"smoke",           el(GAS,     "333333", 0.2,  "e")},
    {"steam",           el(GAS,     "888888", 0.1,  "e")},

    {"fire_source",     el(EMITTER, "FF5A00", 10,   "b")},
    {"water_source",    el(EMITTER, "0E87CC", 10)},
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
    {"water", "ice"},
};
// above 0 temp
std::unordered_map< std::string, std::string > unfreeze {
    {"ice", "water"},
};
// Utility functions for state process
float randf() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}
int randomIncrement() {
    return ( rand() % 2 == 0 ) ? 1 : -1;
}
int intStep(int n, int m, int step) {
    if (n < m && m - n > step) return n + step;
    if (n > m && n - m > step) return n - step; 
    return m;
}
bool tryMove(int x, int y, int x2, int y2, bool gas = false) {
    if( Grid::inBounds(x2, y2) && ( Grid::isEmpty(x2, y2) || Grid::getDensity(x, y) > Grid::getDensity(x2, y2)) ) {
        Grid::switchPixel(x, y, x2, y2);
        return true;
    }
    return false;
}
void tryPlace(int x, int y, std::string element) {
    if( Grid::inBounds(x, y) && Grid::isEmpty(x, y) ) Grid::setPixel(x, y, element, true);
}
// Movement process for each state
// returns 1 if erased/transformed
int universalProcess(int x, int y) {
    if( list[Grid::getElem(x, y)].evaporates && randf() < 0.1 ) {
        auto temp = evap_to.find( Grid::getElem(x, y) );
        if( temp != evap_to.end() )
            Grid::setPixel(x, y, temp->second);
        else Grid::setPixel(x, y, "", true);
        return 1;
    }
    return 0;
}

void solidProcess(int x, int y) {
    if( universalProcess(x, y) ) return;
    if( elem::tryMove(x, y, x, y+1) ) return;
}
void dustProcess(int x, int y) {
    if( universalProcess(x, y) ) return;
    if( elem::tryMove(x, y, x, y+1) ) return;
    int incr = elem::randomIncrement();
    if( elem::tryMove(x, y, x+incr, y+1) ) return;
    if( elem::tryMove(x, y, x-incr, y+1) ) return;
}
void liquidProcess(int x, int y) {
    if( universalProcess(x, y) ) return;
    if( elem::tryMove(x, y, x, y+1) ) return;
    int incr = elem::randomIncrement();
    if( elem::tryMove(x, y, x+incr, y+1) ) return;
    if( elem::tryMove(x, y, x-incr, y+1) ) return;
    if( elem::tryMove(x, y, x+incr, y) ) {
        if( elem::list[ Grid::getElem(x+incr, y) ].fluid ) {
            if( elem::tryMove(x+incr, y, x+2*incr, y+1) ) return;
            if( elem::tryMove(x+incr, y, x+2*incr, y) ) return;
        }
        return;
    }
    if( elem::tryMove(x, y, x-incr, y) ) {
        if( elem::list[ Grid::getElem(x-incr, y) ].fluid ) {
            if( elem::tryMove(x-incr, y, x-2*incr, y+1) ) return;
            if( elem::tryMove(x-incr, y, x-2*incr, y) ) return;
        }
        return;
    }
}
void gasProcess(int x, int y) {
    if( universalProcess(x, y) ) return;
    int incr = elem::randomIncrement();
    if( randf() < 0.8 )
        if( elem::tryMove(x, y, x, y-1) ) return;
    else if( randf() < 0.5 ) {
        if( elem::tryMove(x, y, x+incr, y-1) ) return;
        if( elem::tryMove(x, y, x-incr, y-1) ) return;    
    } else {
        if( elem::tryMove(x, y, x+incr, y) ) return;
        if( elem::tryMove(x, y, x-incr, y) ) return;    
    }
    if( elem::tryMove(x, y, x+incr, y-1) ) return;
    if( elem::tryMove(x, y, x-incr, y-1) ) return;
    if( elem::tryMove(x, y, x+incr, y) ) return;
    if( elem::tryMove(x, y, x-incr, y) ) return;
}
void emitterProcess(int x, int y) {
    if( universalProcess(x, y) ) return;
    std::string to_place = elem::emits[ Grid::getElem(x, y) ];
    tryPlace(x-1, y, to_place);
    tryPlace(x+1, y, to_place);
    tryPlace(x, y-1, to_place);
    tryPlace(x, y+1, to_place);
    return;
}
}
//indexes from states
std::vector<void(*)(int,int)> Grid::stateProcess = {
    &elem::gasProcess,
    &elem::liquidProcess,
    &elem::dustProcess,
    &elem::solidProcess,
    &elem::emitterProcess,
};