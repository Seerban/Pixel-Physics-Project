#include <SFML/Graphics.hpp>

const int WIDTH = 40;
const int HEIGHT = 40;
const int SCALE = 8;

sf::Color grid[HEIGHT][WIDTH];
sf::Image image;
sf::Texture texture;
sf::Sprite sprite;

void setPixel(int x, int y, sf::Color color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        image.setPixel(x, y, color);
        grid[y][x] = color;
        texture.update(image);
    }
}

void swapPixels(int x1, int y1, int x2, int y2) {
    if (
        x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT &&
        x2 >= 0 && x2 < WIDTH && y2 >= 0 && y2 < HEIGHT
    ) {
        std::swap(grid[y1][x1], grid[y2][x2]);
        image.setPixel(x1, y1, grid[y1][x1]);
        image.setPixel(x2, y2, grid[y2][x2]);
        texture.update(image);
    }
}

void gravity() {
    for (int y = HEIGHT - 2; y >= 0; --y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (grid[y][x] != sf::Color::Black && grid[y + 1][x] == sf::Color::Black) {
                swapPixels(x, y, x, y + 1);
            }
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH * SCALE, HEIGHT * SCALE), "Pixel Grid");
    window.setFramerateLimit(30);  // Run at 30 FPS

    image.create(WIDTH, HEIGHT, sf::Color::Black);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    sprite.setScale(SCALE, SCALE);

    setPixel(10, 10, sf::Color::Red);
    setPixel(10, 20, sf::Color::Blue);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        gravity();

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
