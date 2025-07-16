#include "Grid.h"
#include "array"
#include "unordered_map"

using Brush = std::array< std::array< bool, 7 >, 7 >;

Brush tiny_brush {{
    {{0, 0, 0, 0, 0, 0, 0,}},
    {{0, 0, 0, 0, 0, 0, 0,}},
    {{0, 0, 0, 0, 0, 0, 0,}},
    {{0, 0, 0, 1, 0, 0, 0,}},
    {{0, 0, 0, 0, 0, 0, 0,}},
    {{0, 0, 0, 0, 0, 0, 0,}},
    {{0, 0, 0, 0, 0, 0, 0,}},
}};
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
Brush brushes[] { tiny_brush, small_brush, med_brush, big_brush };
unsigned int selected_brush = 1;

std::unordered_map<char, std::string> key_to_elem = {
    {'d', "dirt"},
    {'w', "water"},
    {'f', "fire"},
    {'F', "fuel"},
    {'i', "ice"},
    {'s', "sand"},
    {'S', "steam"},
};
std::string selected_elem = "dirt";

void brush(int x, int y, bool erase=false) {
    for( int i = 0; i < 7; ++i)
        for( int j = 0; j < 7; ++j)
            if( brushes[selected_brush][i][j] && Grid::inBounds(x+j-3, y+i-3) )
                if( !erase ) Grid::setPixel(x+j-3, y+i-3, selected_elem);
                else Grid::setPixel(x+j-3, y+i-3, "");
}

void Grid::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        // exit button
        if (event.type == sf::Event::Closed)
            window.close();
        // place elem event
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i pos = sf::Mouse::getPosition(window);
            brush(pos.x / scale, pos.y / scale);
            return;
        }
        // erase event
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            sf::Vector2i pos = sf::Mouse::getPosition(window);
            brush(pos.x / scale, pos.y / scale, true);
            return;
        }
        // key to select element
        if (event.type == sf::Event::KeyPressed) {
            int key = int(event.key.code + 97);
            // 124 is '0' because of the added value for letter chars
            if(key >= 124 && key <= 127) {
                selected_brush = key - 124;
                std::cout<<"selected brush size "<< selected_brush << std::endl;
                return;
            }
            // capitalize if shift
            if(event.key.shift) key -= 32;

            // see if key to elem entry exists
            auto it = key_to_elem.find(key);
            bool found = it != key_to_elem.end();
            if( found ) {
                std::cout<< "selected "<< key_to_elem[key] << std::endl;
                selected_elem = key_to_elem[key];
            }
        }
    }
}