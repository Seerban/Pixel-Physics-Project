#include "Grid.h"

void Grid::moveSolid(int x, int y) { return; }
void Grid::moveDust(int x, int y) {
    if( inBounds(x, y+1) && isEmpty(x, y+1) ) switchPixel(x, y, x, y+1);
}
void Grid::moveLiquid(int x, int y) { 
    if( inBounds(x, y+1) && isEmpty(x, y+1) ) switchPixel(x, y, x, y+1);
}
void Grid::moveGas(int x, int y) {
    if( inBounds(x, y+1) && isEmpty(x, y-1) ) switchPixel(x, y, x, y-1);
}

std::unordered_map<State, void(Grid::*)(int,int)> Grid::moveFunc = {
    {SOLID,  &Grid::moveSolid},
    {DUST,   &Grid::moveDust},
    {LIQUID, &Grid::moveLiquid},
    {GAS,    &Grid::moveGas},
};