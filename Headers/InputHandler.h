#include "Grid.h"
#include <unordered_map>
#include <string>

//std::unordered_map<char, std::string> key_to_elem;

class InputHandler {
    int size, scale, brush_size;
    std::string selected_elem = "dirt";
    sf::Vector2i last_tick_placed = sf::Vector2i(-1, -1);

    sf::RenderWindow *window;
    Grid *g;

public: 
    InputHandler(int size, int scale, Grid& g, sf::RenderWindow &window);

    void process();
    void brush(int x, int y, bool erase=false);
    void brush_line(int x1, int y1, int x2, int y2, bool erase=false);
};
