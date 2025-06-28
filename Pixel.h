#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#pragma once

class Pixel {
    sf::Color col;
public:
    static std::vector<std::vector< Pixel* >>  grid;
    static sf::Image image;
    static sf::Texture texture;
    static sf::Sprite sprite;

    Pixel() : col(sf::Color::Black) {}
    Pixel(sf::Color col) : col(col) {}

    void setGrid(std::vector<std::vector< Pixel* >> g) { this->grid = g; }
    sf::Color getCol() const { return this->col; }
    void setCol(sf::Color color) { this->col = color; }

    friend std::ostream& operator<<(std::ostream& o, Pixel& p) {
        o << static_cast<int>(p.getCol().r)
            << static_cast<int>(p.getCol().g)
            << static_cast<int>(p.getCol().b)
            << ' ';
        return o;
    }

    void render(int x, int y) {
        image.setPixel(x, y, getCol() );
    }
    static void switch_pixel(int x1, int y1, int x2, int y2) {
        std::swap(grid[y1][x1], grid[y2][x2]);
        grid[y1][x1]->render(x1, y1);
        grid[y2][x2]->render(x2, y2);
    } 
    virtual void process(int x, int y) {}
};