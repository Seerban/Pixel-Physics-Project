#include "States.h"
#include "Element.h"
#include "Grid.h"

bool tryMove(Grid &g, int x, int y, int x2, int y2, bool gas) {
    if( g.inBounds(x2, y2) && ( g.isEmpty(x2, y2) || g.getDensity(x, y) > g.getDensity(x2, y2)) ) {
        g.switchPixel(x, y, x2, y2);
        return true;
    }
    return false;
}
void tryPlace(Grid &g, int x, int y, std::string element) {
    if( g.inBounds(x, y) && g.isEmpty(x, y) ) g.setPixel(x, y, element, true);
}

int state::universalProcess(Grid &g, int x, int y) {
    std::string element = g.getElem(x, y);
    if( elem::list[element].evaporates && elemutil::randf() < elem::list[element].evap_chance ) {
        auto temp = elem::evap_to.find( g.getElem(x, y) );
        if( temp != elem::evap_to.end() )
            g.setPixel(x, y, temp->second);
        else g.setPixel(x, y, "", true);
        return 1;
    }
    return 0;
}
void state::solidProcess(Grid &g, int x, int y) {
    if( universalProcess(g, x, y) ) return;
    if( tryMove(g, x, y, x, y+1) ) return;
}
void state::dustProcess(Grid &g, int x, int y) {
    if( universalProcess(g, x, y) ) return;
    if( tryMove(g, x, y, x, y+1) ) return;
    int incr = elemutil::randomIncrement();
    if( tryMove(g, x, y, x+incr, y+1) ) return;
    if( tryMove(g, x, y, x-incr, y+1) ) return;
}
void state::liquidProcess(Grid &g, int x, int y) {
    if( universalProcess(g, x, y) ) return;
    if( tryMove(g, x, y, x, y+1) ) return;
    int incr = elemutil::randomIncrement();
    if( tryMove(g, x, y, x+incr, y+1) ) return;
    if( tryMove(g, x, y, x-incr, y+1) ) return;
    if( tryMove(g, x, y, x+incr, y) ) {
        if( elem::list[ g.getElem(x+incr, y) ].fluid ) {
            if( tryMove(g, x+incr, y, x+2*incr, y+1) ) return;
            if( tryMove(g, x+incr, y, x+2*incr, y) ) return;
        }
        return;
    }
    if( tryMove(g, x, y, x-incr, y) ) {
        if( elem::list[ g.getElem(x-incr, y) ].fluid ) {
            if( tryMove(g, x-incr, y, x-2*incr, y+1) ) return;
            if( tryMove(g, x-incr, y, x-2*incr, y) ) return;
        }
        return;
    }
}
void state::gasProcess(Grid &g, int x, int y) {
    if( universalProcess(g, x, y) ) return;
    int incr = elemutil::randomIncrement();
    if( elemutil::randf() < 0.8 )
        if( tryMove(g, x, y, x, y-1) ) return;
    else if( elemutil::randf() < 0.5 ) {
        if( tryMove(g, x, y, x+incr, y-1) ) return;
        if( tryMove(g, x, y, x-incr, y-1) ) return;    
    } else {
        if( tryMove(g, x, y, x+incr, y) ) return;
        if( tryMove(g, x, y, x-incr, y) ) return;    
    }
    if( tryMove(g, x, y, x+incr, y-1) ) return;
    if( tryMove(g, x, y, x-incr, y-1) ) return;
    if( tryMove(g, x, y, x+incr, y) ) return;
    if( tryMove(g, x, y, x-incr, y) ) return;
}
void state::neutralProcess(Grid &g, int x, int y) {
    if( universalProcess(g, x, y) ) return;
    int hincr = elemutil::randomIncrement();
    int vincr = elemutil::randomIncrement();
    if( elemutil::randf() < 0.33 ) {
        if( tryMove(g, x, y, x+hincr, y) ) return;
        if( tryMove(g, x, y, x-hincr, y+vincr) ) return;
    }
    if( tryMove(g, x, y, x+hincr, y+vincr) ) return;
    if( tryMove(g, x, y, x, y+vincr) ) return;
    if( tryMove(g, x, y, x-hincr, y+vincr) ) return;
    if( tryMove(g, x, y, x+hincr, y-vincr) ) return;
    if( tryMove(g, x, y, x, y+vincr) ) return;
    if( tryMove(g, x, y, x-hincr, y-vincr) ) return;
}
void state::emitterProcess(Grid &g, int x, int y) {
    std::string to_place = elem::emits[ g.getElem(x, y) ];
    tryPlace(g, x-1, y, to_place);
    tryPlace(g, x+1, y, to_place);
    tryPlace(g, x, y-1, to_place);
    tryPlace(g, x, y+1, to_place);
    if( universalProcess(g, x, y) ) return; // placed after so it can spawn before evaporating
    return;
}
void state::lifeformProcess(Grid &g, int x, int y) {
    if( universalProcess(g, x, y) ) return;
    if( tryMove(g, x, y, x, y+1) ) return;
    //if( elemutil:: randf() > 0.5 ) return; // chance to not move
    int incr = elemutil::randomIncrement();
    if( tryMove(g, x, y, x+incr, y) ) return;
    if( tryMove(g, x, y, x+incr, y-1) ) return;
    if( tryMove(g, x, y, x-incr, y) ) return;
    if( tryMove(g, x, y, x-incr, y-1) ) return;
}
void state::staticProcess(Grid &g, int x, int y) {
    universalProcess(g, x, y);
    return;
}
void state::explosionProcess(Grid &g, int x, int y) {
    std::string s = g.getElem(x, y);
    int size = int(s[s.length() - 1] - '0');  // size is the radius
    int radius = size*size;
    int temp = g.getTemp(x, y);
    g.setPixel(x, y, "explosion", true); // normalize to explosion for mutual reactions

    for (int i = -radius; i <= radius; ++i) {
        for (int j = -radius; j <= radius; ++j) {
            if (i*i + j*j <= radius * radius) {
                if( g.inBounds(x+i, y+j) ) {
                    if( g.isEmpty(x+i, y+j) || g.getElem(x+i, y+j) == "fire")
                        g.setPixel(x + i, y + j, "fire", true);
                    else {
                        // lower temperature to half the furthere away it is
                        float dist = std::sqrt(i * i + j * j);
                        float normDist = dist / radius / 2;
                        int localTemp = int(temp * (1. - normDist));
                        g.checkReaction(x, y, x+i, y+j, true);
                        g.setTemp(x+i, y+j, localTemp );
                    }
                }
            }
        }
    }
    // Also set the center pixel
    g.setPixel(x, y, "fire", true);
}

std::vector<void(*)(Grid&,int,int)> state::stateProcess = {
    &gasProcess,
    &neutralProcess,
    &liquidProcess,
    &dustProcess,
    &solidProcess,
    &emitterProcess,
    &lifeformProcess,
    &staticProcess,
    &explosionProcess,
};