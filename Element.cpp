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
    if( strchr(tags, 'b') ) e.burning = true;
    if( strchr(tags, 'e') ) e.evaporates = true;
    if( strchr(tags, 'f') ) e.fluid = true;
    return e;
}

// List of elements
std::unordered_map< std::string, element > list {
    {"",                el(SOLID,   "000000", 0.5)},
    {"dirt",            el(SOLID,   "964B00", 2)},
    {"glass",           el(SOLID,   "DDDDDD", 2)},
    {"ice",             el(SOLID,   "AAAAFF", 2)},
    {"rock",            el(SOLID,   "555555", 2)},
    {"wet_sand",        el(SOLID,   "A28260", 2)},

    {"burning_gasoline",el(LIQUID,  "FF2222", 0.5,  "be")},
    {"gasoline",        el(LIQUID,  "151555", 0.5)},
    {"water",           el(LIQUID,  "0E87CC", 1,    "f")},

    {"gravel",          el(DUST,    "999999", 2)},
    {"sand",            el(DUST,    "C2B280", 2)},
    {"mud",             el(DUST,    "70543E", 2)},

    {"fire",            el(GAS,     "FF5A00", 0.1, "be")},
    {"smoke",           el(GAS,     "333333", 0.2, "e")},
    {"steam",           el(GAS,     "888888", 0.3, "e")},

    {"fire_source",     el(EMITTER, "FF5000", 1, "b")},
    {"water_source",    el(EMITTER, "0080C0", 1)},
};
// Element Reactions
std::unordered_map< std::string, std::unordered_map< std::string, std::pair< std::string, float > > > reaction {
    {"dirt",    { {"water", {"mud", 0.5}}, }},
    {"fire",    { {"water", {"", 1}}, {"ice", {"", 1}},  }},
    {"sand",    { {"water", {"wet_sand", 1}}, }},
    {"water",   { {"dirt",  {"", 1}}, }},

};
// Element emits other Elem
std::unordered_map< std::string, std::string > emits {
    {"fire_source",     "fire"},
    {"water_source",    "water"},
};
// reaction with burning element
std::unordered_map< std::string, std::string > melt {
    {"gasoline",    "burning_gasoline"},
    {"ice",     "water"},
    {"sand",    "glass"},
    {"water",   "steam"},
};

std::unordered_map< std::string, std::string > evap_to {
    {"fire", "smoke"},
};
// Utility functions for state process
float randf() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}
int randomIncrement() {
    return ( rand() % 2 == 0 ) ? 1 : -1;
}
bool tryMove(int x, int y, int x2, int y2, bool gas = false) {
    if( Grid::inBounds(x2, y2) && ( Grid::isEmpty(x2, y2) || Grid::getDensity(x, y) > Grid::getDensity(x2, y2)) ) {
        Grid::switchPixel(x, y, x2, y2);
        return true;
    }
    return false;
}
void tryPlace(int x, int y, std::string element) {
    if( Grid::inBounds(x, y) && Grid::isEmpty(x, y) ) Grid::setPixel(x, y, element);
}
// Movement process for each state
// returns 1 if erased/transformed
int universalProcess(int x, int y) {
    if( list[Grid::getElem(x, y)].evaporates && randf() < 0.2 ) {
        auto temp = evap_to.find( Grid::getElem(x, y) );
        if( temp != evap_to.end() )
            Grid::setPixel(x, y, temp->second);
        else Grid::setPixel(x, y, "");
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
    if( randf() < 0.85 )
        if( elem::tryMove(x, y, x, y-1) ) return;
    else {
        if( elem::tryMove(x, y, x+incr, y-1) ) return;
        if( elem::tryMove(x, y, x-incr, y-1) ) return;    
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