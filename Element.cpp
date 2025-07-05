#include "Element.h"
#include "Grid.h"

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
std::unordered_map< std::string, std::string > emits {
    {"fire_source",     "fire"},
    {"water_source",    "water"},
};

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

void solidProcess(int x, int y) { 
    return;
}
void dustProcess(int x, int y) {
    if( elem::tryMove(x, y, x, y+1) ) return;
    int incr = elem::randomIncrement();
    if( elem::tryMove(x, y, x+incr, y+1) ) return;
    if( elem::tryMove(x, y, x-incr, y+1) ) return;
}
void liquidProcess(int x, int y) { 
    if( elem::tryMove(x, y, x, y+1) ) return;
    int incr = elem::randomIncrement();
    if( elem::tryMove(x, y, x+incr, y+1) ) return;
    if( elem::tryMove(x, y, x-incr, y+1) ) return;
    if( elem::tryMove(x, y, x+incr, y) ) return;
    if( elem::tryMove(x, y, x+incr, y) ) return;
}
void gasProcess(int x, int y) {
    if( elem::tryMove(x, y, x, y-1) ) return;
    int incr = elem::randomIncrement();
    if( elem::tryMove(x, y, x+incr, y-1) ) return;
    if( elem::tryMove(x, y, x-incr, y-1) ) return;
    if( elem::tryMove(x, y, x+incr, y) ) return;
    if( elem::tryMove(x, y, x+incr, y) ) return;
}
void emitterProcess(int x, int y) {
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