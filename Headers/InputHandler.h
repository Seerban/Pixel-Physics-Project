#include "Grid.h"
#include <unordered_map>
#include <string>

//std::unordered_map<char, std::string> key_to_elem;

class InputHandler {
    int size, scale, brush_size;
    std::string selected_elem = "dirt";

    sf::RenderWindow *window;
    Grid *g;

public: 
    InputHandler(int size, int scale, Grid& g, sf::RenderWindow &window);

    void process();
    void brush(int x, int y, bool erase=false);

};
