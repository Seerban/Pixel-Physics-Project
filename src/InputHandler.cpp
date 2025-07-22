#include "InputHandler.h"

std::vector<sf::Vector2i> getLinePoints(int x1, int y1, int x2, int y2) {
    std::vector<sf::Vector2i> points;

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;

    int err = dx - dy;

    while (true) {
        points.emplace_back(x1, y1);

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }

    return points;
}

std::unordered_map<char, std::string> key_to_elem = {
    {'d', "dirt"},
    {'r', "rock"},
    {'i', "ice"},

    {'s', "sand"},
    {'g', "gravel"},

    {'w', "water"},
    {'W', "water_source"},
    {'o', "oil"},
    {'l', "lava"},
    
    {'f', "fire"},
    {'F', "fire_source"},
    {'p', "plasma"},

    {'b', "bug"},
    {'G', "grass_seed"},
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

void InputHandler::brush_line(int x1, int y1, int x2, int y2, bool erase) {
    std::vector<sf::Vector2i> line = getLinePoints(x1, y1, x2, y2);
    for (const auto& p : line) {
        brush(p.x, p.y, erase);
    }
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

            if( last_tick_placed == sf::Vector2i(-1, -1) )
                brush(pos.x / scale, pos.y / scale);
            else
                brush_line(last_tick_placed.x, last_tick_placed.y, pos.x / scale, pos.y / scale);
            last_tick_placed = pos/scale;
            return;
        }
        // erase event
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            sf::Vector2i pos = sf::Mouse::getPosition(*window);

            if( last_tick_placed == sf::Vector2i(-1, -1) )
                brush(pos.x / scale, pos.y / scale, true);
            else
                brush_line(last_tick_placed.x, last_tick_placed.y, pos.x / scale, pos.y / scale, true);
            last_tick_placed = pos / scale;
            return;
        } else last_tick_placed = sf::Vector2i(-1, -1);
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