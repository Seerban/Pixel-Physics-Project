#include "Grid.h"
#include "UIHandler.h"
#include "InputHandler.h"

const int SIZE = 80; //multiple of CHUNK size in grid.h
const int SCALE = 6;
const int FPS = 30;

int main() {
    srand(time(0));
    sf::RenderWindow window;
    sf::Image image; // manipulated inside grid
    sf::Texture texture;
    sf::Sprite sprite;
    image.create(SIZE, SIZE, sf::Color::Black);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    sprite.setScale(SCALE, SCALE);
    window.create( sf::VideoMode(SIZE*SCALE, SIZE*SCALE), "Grid" );
    window.setFramerateLimit( FPS );

    //show_controls();
    Grid grid(SIZE, SCALE, FPS, image, window);
    UIHandler ui(SIZE, SCALE, window, grid);
    InputHandler input(SIZE, SCALE, grid, window);

    while (window.isOpen()) {
            grid.process();
            input.process();
            
            texture.update(image);
            window.clear();
            
            window.draw(sprite);
            ui.draw();

            window.display();
        }
}
