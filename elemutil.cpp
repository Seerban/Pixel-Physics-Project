#include <random>

namespace elemutil {
    float randf() {
        return float(rand()) / float(RAND_MAX);
    }
    int randomIncrement() {
        return ( rand() % 2 == 0 ) ? 1 : -1;
    }
    int intStep(int n, int m, int step) {
        if (n < m && m - n > step) return n + step;
        if (n > m && n - m > step) return n - step; 
        return m;
    }
}