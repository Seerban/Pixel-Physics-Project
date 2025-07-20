#include "States.h"
#include "Element.h"
#include "Grid.h"

int State::universalProcess(int x, int y) {
    if( elem::list[Grid::getElem(x, y)].evaporates && randf() < 0.1 ) {
        auto temp = elem::evap_to.find( Grid::getElem(x, y) );
        if( temp != elem::evap_to.end() )
            Grid::setPixel(x, y, temp->second);
        else Grid::setPixel(x, y, "", true);
        return 1;
    }
    return 0;
}

void State::solidProcess(int x, int y) {
    if( universalProcess(x, y) ) return;
    if( elem::tryMove(x, y, x, y+1) ) return;
}
void State::dustProcess(int x, int y) {
    if( universalProcess(x, y) ) return;
    if( elem::tryMove(x, y, x, y+1) ) return;
    int incr = elem::randomIncrement();
    if( elem::tryMove(x, y, x+incr, y+1) ) return;
    if( elem::tryMove(x, y, x-incr, y+1) ) return;
}
void State::liquidProcess(int x, int y) {
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
void State::gasProcess(int x, int y) {
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
void State::emitterProcess(int x, int y) {
    if( universalProcess(x, y) ) return;
    std::string to_place = elem::emits[ Grid::getElem(x, y) ];
    elem::tryPlace(x-1, y, to_place);
    elem::tryPlace(x+1, y, to_place);
    elem::tryPlace(x, y-1, to_place);
    elem::tryPlace(x, y+1, to_place);
    return;
}
