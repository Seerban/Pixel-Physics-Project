#include "Input.h"
#include "Grid.h"
#include "array"

using Brush = std::array< std::array< bool, 7 >, 7 >;

Brush small_brush {{
    {{0, 0, 0, 0, 0, 0, 0,}},
    {{0, 0, 0, 0, 0, 0, 0,}},
    {{0, 0, 0, 1, 0, 0, 0,}},
    {{0, 0, 1, 1, 1, 0, 0,}},
    {{0, 0, 0, 1, 0, 0, 0,}},
    {{0, 0, 0, 0, 0, 0, 0,}},
    {{0, 0, 0, 0, 0, 0, 0,}},
}};
Brush med_brush {{
    {{0, 0, 0, 0, 0, 0, 0,}},
    {{0, 0, 0, 1, 0, 0, 0,}},
    {{0, 0, 1, 1, 1, 0, 0,}},
    {{0, 1, 1, 1, 1, 1, 0,}},
    {{0, 0, 1, 1, 1, 0, 0,}},
    {{0, 0, 0, 1, 0, 0, 0,}},
    {{0, 0, 0, 0, 0, 0, 0,}},
}};
Brush big_brush {{
    {{0, 0, 0, 1, 0, 0, 0,}},
    {{0, 0, 1, 1, 1, 0, 0,}},
    {{0, 1, 1, 1, 1, 1, 0,}},
    {{1, 1, 1, 1, 1, 1, 1,}},
    {{0, 1, 1, 1, 1, 1, 0,}},
    {{0, 0, 1, 1, 1, 0, 0,}},
    {{0, 0, 0, 1, 0, 0, 0,}},
}};
Brush brushes[] { small_brush, med_brush, big_brush };
unsigned int selected_brush = 1;


void brush(int x, int y, bool erase=false) {
    for( int i = 0; i < 7; ++i)
        for( int j = 0; j < 7; ++j)
            if( brushes[selected_brush][i][j] && Grid::inBounds(x+j-3, y+i-3) )
                if( !erase ) Grid::setPixel(x+j-3, y+i-3, "dirt");
                else Grid::setPixel(x+j-3, y+i-3, "");
}

void Grid::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x / scale;
                int y = event.mouseButton.y / scale;
                brush(x, y);
            }
            if (event.mouseButton.button == sf::Mouse::Right) {
                int x = event.mouseButton.x / scale;
                int y = event.mouseButton.y / scale;
                brush(x, y, true);
            }
        }
    }
}