#include "Grid.h"

// INITIALIZES GRID STATICS

int Grid::size;
int Grid::scale;

std::vector<std::vector<Pixel>> Grid::grid;
std::vector<std::vector<sf::Color>> Grid::debug_grid;
std::vector<std::vector<bool>> Grid::active_chunks;

sf::Image Grid::image;
sf::Texture Grid::texture;
sf::Sprite Grid::sprite;
sf::RenderWindow Grid::window;

sf::Text Grid::cornerText;
sf::Text Grid::cornerText2;

float randf() {
    return double(random()) / RAND_MAX;
}