#include "Grid.h"

// INITIALIZES GRID STATICS

int Grid::size;
int Grid::scale;
std::vector<std::vector<Pixel>> Grid::grid;
sf::Image Grid::image;
sf::Texture Grid::texture;
sf::Sprite Grid::sprite;
sf::RenderWindow Grid::window;