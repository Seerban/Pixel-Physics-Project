#include "InputHandler.h"

std::unordered_map<char, std::string> key_to_elem = {
    {'q', "dirt"},
    {'w', "rock"},
    {'e', "ice"},
    {'r', "mud"},

    {'a', "sand"},
    {'s', "gravel"},

    {'z', "water"},
    {'Z', "water_source"},
    {'x', "gasoline"},
    {'c', "lava"},
    
    {'f', "fire"},
    {'F', "fire_source"},
    {'g', "plasma"},
    {'h', "steam"},

    {'b', "bug"},
    {'n', "seed"},
};

InputHandler::InputHandler(int size, int scale, Grid& g, sf::RenderWindow &window) {
    this->size = size;
    this->scale = scale;
    this->window = &window;
    this->g = &g;
    this->brush_size = 5;
}

void InputHandler::brush(int x, int y, bool erase) {
    for(int i = -brush_size; i <= brush_size; ++i)
        for(int j = -brush_size; j <= brush_size; ++j)
            if( i*i+j*j<0.75*brush_size*brush_size && g->inBounds(x+j, y+i))
                if( !erase ) { if(g->isEmpty(x+j, y+i) ) g->setPixel(x+j, y+i, selected_elem, true); }
                else g->setPixel(x+j, y+i, "", true);
}

void InputHandler::process() {
    sf::Event event;
    while (window->pollEvent(event)) {
        // exit button
        if (event.type == sf::Event::Closed)
            window->close();
        // place elem event
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i pos = sf::Mouse::getPosition(*window);
            brush(pos.x / scale, pos.y / scale);
            return;
        }
        // erase event
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            sf::Vector2i pos = sf::Mouse::getPosition(*window);
            brush(pos.x / scale, pos.y / scale, true);
            return;
        }
        // key to select element/debug
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::F1) {
                g->clearDebug();
                g->debug = !g->debug;
                return;
            }
            // assume key to select elem otherwise
            int key = int(event.key.code + 97);
            // 124 is '0' because of the added value for letter chars
            if(key >= 124 && key <= 133) {
                brush_size = key - 123;
                //std::cout<<"selected brush size "<< brush_size << std::endl;
                return;
            }
            // capitalize if shift
            if(event.key.shift) key -= 32;

            // see if key to elem entry exists
            auto it = key_to_elem.find(key);
            bool found = it != key_to_elem.end();
            if( found ) {
                //std::cout<< "selected "<< key_to_elem[key] << std::endl;
                selected_elem = key_to_elem[key];
            }
        }
    }
}