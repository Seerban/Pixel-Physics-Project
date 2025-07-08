#include "Element.h"
#include "Grid.h"
#include <cstdlib>

// contains list of elements

namespace elem {

element el(State state, const char* hex) {
    element e;
    int r, g, b;
    sscanf(hex, "%02x%02x%02x", &r, &g, &b);
    e.col = sf::Color(r,g,b);
    e.state = state;
    return e;
}

// List of elements
std::unordered_map< std::string, element > list {
    {"",        el(SOLID,   "000000")},
    {"dirt",    el(SOLID,   "964B00")},

    {"water",   el(LIQUID,  "0E87CC")},

    {"sand",    el(DUST,    "C2B280")},

    {"fire",    el(GAS,     "FF5A00")},
    {"steam",   el(GAS,     "888888")},

    {"fire_source",     el(EMITTER, "FF0000")},
    {"water_source",    el(EMITTER, "0000FF")},
};
// Element Reactions
std::unordered_map< std::string, std::unordered_map< std::string, std::string > > reaction {
    {"fire",    {{"water", ""}}, },
    {"water",   {{"fire", "steam"}}, },
};
// Element emits other Elem
std::unordered_map< std::string, std::string > emits {
    {"fire_source",     "fire"},
    {"water_source",    "water"},
};
// Chance to disappear when exposed
std::unordered_map< std::string, float > evaporate {
    {"steam", 0.1},
    {"fire", 0.1},
};

// Utility functions for state process
float randf() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}
int randomIncrement() {
    return ( random() % 2 == 0 ) ? 1 : -1;
}
bool tryMove(int x, int y, int x2, int y2) {
    if( Grid::inBounds(x2, y2) && Grid::isEmpty(x2, y2) ) {
        Grid::switchPixel(x, y, x2, y2);
        return true;
    }
    return false;
}
void tryPlace(int x, int y, std::string element) {
    if( Grid::inBounds(x, y) && Grid::isEmpty(x, y) ) Grid::setPixel(x, y, element);
}
// Movement process for each state
// returns 1 if erased
int universalProcess(int x, int y) {
    if( randf() < evaporate[ Grid::getElem(x, y) ] ) {
        Grid::setPixel(x, y, "");
        return 1;
    }
    return 0;
}

void solidProcess(int x, int y) {
    if( universalProcess(x, y) ) return;
    return;
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
    if( elem::tryMove(x, y, x+incr, y) ) return;
    if( elem::tryMove(x, y, x+incr, y) ) return;
}
void gasProcess(int x, int y) {
    if( universalProcess(x, y) ) return;
    if( elem::tryMove(x, y, x, y-1) ) return;
    int incr = elem::randomIncrement();
    if( elem::tryMove(x, y, x+incr, y-1) ) return;
    if( elem::tryMove(x, y, x-incr, y-1) ) return;
    if( elem::tryMove(x, y, x+incr, y) ) return;
    if( elem::tryMove(x, y, x+incr, y) ) return;
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
    &elem::solidProcess,
    &elem::dustProcess,
    &elem::liquidProcess,
    &elem::gasProcess,
    &elem::emitterProcess,
};