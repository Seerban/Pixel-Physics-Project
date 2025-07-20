#pragma once
#include <vector>
#include <string>

class Grid;

namespace state {

enum States {
    GAS,
    LIQUID, 
    DUST, 
    SOLID, 
    EMITTER, // "spawns" other pixels and is SOLID
};

// state processes
int universalProcess(Grid &g, int x, int y);
void solidProcess(Grid &g, int x, int y);
void dustProcess(Grid &g, int x, int y);
void liquidProcess(Grid &g, int x, int y);
void gasProcess(Grid &g, int x, int y);
void emitterProcess(Grid &g, int x, int y);

extern std::vector<void(*)(Grid&,int,int)> stateProcess;
}

// utility functions for movement
void tryPlace(Grid &g, int x, int y, std::string element);
bool tryMove(Grid &g, int x, int y, int x2, int y2, bool gas = false);