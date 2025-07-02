#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream> // debugging
#include "Pixel.h"
#include "Elements.h"
#include "Liquid.h"
#include "Gas.h"
#include "Dust.h"

const int WIDTH = 40;
const int HEIGHT = 40;
const int SCALE = 8;
const int FPS = 16;

std::vector<std::vector< Pixel* >> Pixel::grid;
sf::Image Pixel::image;
sf::Texture Pixel::texture;
sf::Sprite Pixel::sprite;

// temporary until pixel service
void setPixel(int x, int y, std::string p) {
   Pixel::setPixel(x,y,p);
}

int main() {
    Pixel::grid.resize(HEIGHT, std::vector< Pixel* >(WIDTH));
    
    Pixel::set_seed();
    
    sf::RenderWindow window(sf::VideoMode(WIDTH * SCALE, HEIGHT * SCALE), "Pixel Grid");
    window.setFramerateLimit(FPS);
    
    Pixel::image.create(WIDTH, HEIGHT, sf::Color::Black);
    Pixel::texture.loadFromImage(Pixel::image);
    Pixel::sprite.setTexture(Pixel::texture);
    Pixel::sprite.setScale(SCALE, SCALE);
    
    for (int y = 0; y < HEIGHT; ++y)
        for (int x = 0; x < WIDTH; ++x)
            setPixel(x, y, "empty");

    setPixel(10, 10, "water");
    setPixel(11, 10, "water");
    setPixel(10, 11, "water");
    setPixel(10, 9, "water");
    setPixel(20, 10, "sand");
    setPixel(19, 10, "sand");
    setPixel(21, 10, "sand");
    setPixel(20, 7, "steam");
    setPixel(10, 30, "earth");
    setPixel(15, 15, "fire_source");

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
