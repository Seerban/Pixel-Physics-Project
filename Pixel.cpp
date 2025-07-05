#include "Pixel.h"

sf::Color randomize_col(sf::Color col) {
    // random col variation
    int dr = (rand() % COL_VAR*2+1) - COL_VAR;
    int dg = (rand() % COL_VAR*2+1) - COL_VAR;
    int db = (rand() % COL_VAR*2+1) - COL_VAR;

    int r = std::clamp(static_cast<int>(col.r) + dr, 0, 255);
    int g = std::clamp(static_cast<int>(col.g) + dg, 0, 255);
    int b = std::clamp(static_cast<int>(col.b) + db, 0, 255);

    return sf::Color(r, g, b);
}
