#include "Grid.h"

const int SIZE = 80; //multiple of 8
const int SCALE = 6;
const int FPS = 30;

int main() {
    Grid grid(SIZE, SCALE, FPS);
    grid.start();
}
