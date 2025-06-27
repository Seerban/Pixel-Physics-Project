#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Pixel.h"

const int WIDTH = 40;
const int HEIGHT = 40;
const int SCALE = 8;

std::vector<std::vector<Pixel>> Pixel::grid;
sf::Image Pixel::image;
sf::Texture Pixel::texture;
sf::Sprite Pixel::sprite;

void setPixel(int x, int y, Pixel p) {
   Pixel::grid[y][x] = p;
   p.render(x, y);
}

int main() {
    Pixel::grid.resize(HEIGHT, std::vector<Pixel>(WIDTH));

    sf::RenderWindow window(sf::VideoMode(WIDTH * SCALE, HEIGHT * SCALE), "Pixel Grid");
    window.setFramerateLimit(30);  // Run at 30 FPS

    Pixel::image.create(WIDTH, HEIGHT, sf::Color::Black);
    Pixel::texture.loadFromImage(Pixel::image);
    Pixel::sprite.setTexture(Pixel::texture);
    Pixel::sprite.setScale(SCALE, SCALE);

    setPixel(10, 10, Pixel(sf::Color::Red) );
    setPixel(10, 20, Pixel(sf::Color::Blue) );
    setPixel(20, 10, Pixel(sf::Color::Green) );

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        Pixel::texture.update(Pixel::image);

        window.clear();
        window.draw(Pixel::sprite);
        window.display();
    }

    return 0;
}
