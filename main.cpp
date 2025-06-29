#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream> // debugging
#include "Elements.h"

const int WIDTH = 40;
const int HEIGHT = 40;
const int SCALE = 8;
const int FPS = 16;

std::vector<std::vector< Pixel* >> Pixel::grid;
sf::Image Pixel::image;
sf::Texture Pixel::texture;
sf::Sprite Pixel::sprite;

// temporary until pixel service
void setPixel(int x, int y, Pixel* p) {
   Pixel::grid[y][x] = p;
   p->render(x, y);
}

int main() {
    Pixel::grid.resize(HEIGHT, std::vector< Pixel* >(WIDTH));
    for (int y = 0; y < HEIGHT; ++y)
        for (int x = 0; x < WIDTH; ++x)
            Pixel::grid[y][x] = new Pixel();

    sf::RenderWindow window(sf::VideoMode(WIDTH * SCALE, HEIGHT * SCALE), "Pixel Grid");
    window.setFramerateLimit(FPS);

    Pixel::image.create(WIDTH, HEIGHT, sf::Color::Black);
    Pixel::texture.loadFromImage(Pixel::image);
    Pixel::sprite.setTexture(Pixel::texture);
    Pixel::sprite.setScale(SCALE, SCALE);

    setPixel(10, 10, elements["water"]->clone() );
    setPixel(11, 10, elements["water"]->clone() );
    setPixel(10, 11, elements["water"]->clone() );
    setPixel(10, 9, elements["water"]->clone() );
    setPixel(20, 10, elements["sand"]->clone() );
    setPixel(19, 10, elements["sand"]->clone() );
    setPixel(21, 10, elements["sand"]->clone() );
    setPixel(20, 7, elements["steam"]->clone() );

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        Pixel::processPixels();
        Pixel::texture.update(Pixel::image);

        window.clear();
        window.draw(Pixel::sprite);
        window.display();
    }

    return 0;
}
