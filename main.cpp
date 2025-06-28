#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream> // debugging
#include "Pixel.h"
#include "Dust.h"
#include "Liquid.h"
#include "Gas.h"

const int WIDTH = 40;
const int HEIGHT = 40;
const int SCALE = 8;
const int FPS = 10;

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

    setPixel(10, 10, new Pixel(sf::Color::Red) );
    setPixel(10, 20, new Pixel(sf::Color::Blue) );
    setPixel(20, 10, new Pixel(sf::Color::Green) );
    setPixel(30, 10, new Dust(sf::Color::Magenta) );
    setPixel(30, 8, new Dust(sf::Color::Cyan) );
    setPixel(30, 7, new Dust(sf::Color::Yellow) );
    setPixel(35, 3, new Liquid(sf::Color(35, 75, 255, 255)));
    setPixel(25, 20, new Gas(sf::Color(175, 175, 175, 255)));
    Pixel::switch_pixel(30, 10, 30, 11);

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
