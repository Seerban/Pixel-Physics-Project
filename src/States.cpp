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
    if( elem::list[g.getElem(x, y)].evaporates && elemutil::randf() < 0.1 ) {
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
void state::emitterProcess(Grid &g, int x, int y) {
    if( universalProcess(g, x, y) ) return;
    std::string to_place = elem::emits[ g.getElem(x, y) ];
    tryPlace(g, x-1, y, to_place);
    tryPlace(g, x+1, y, to_place);
    tryPlace(g, x, y-1, to_place);
    tryPlace(g, x, y+1, to_place);
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

std::vector<void(*)(Grid&,int,int)> state::stateProcess = {
    &gasProcess,
    &liquidProcess,
    &dustProcess,
    &solidProcess,
    &emitterProcess,
    &lifeformProcess,
};