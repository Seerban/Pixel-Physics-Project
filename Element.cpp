#include "Element.h"

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
};

}