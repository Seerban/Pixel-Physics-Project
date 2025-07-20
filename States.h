#pragma once

namespace State {

enum State {
    GAS,
    LIQUID, 
    DUST, 
    SOLID, 
    EMITTER, // "spawns" other pixels and is SOLID
};

// state processes
int universalProcess(int x, int y);
void solidProcess(int x, int y);
void dustProcess(int x, int y);
void liquidProcess(int x, int y);
void gasProcess(int x, int y);
void emitterProcess(int x, int y);

}