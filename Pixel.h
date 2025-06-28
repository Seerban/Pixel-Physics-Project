#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream> // debug
#include <cstdlib> // randomizer
#include <ctime> // "random" seed
#pragma once

class Pixel {
    sf::Color col;
public:
    static std::vector<std::vector< Pixel* >>  grid;
    static sf::Image image;
    static sf::Texture texture;
    static sf::Sprite sprite;
    // Constructor
    Pixel() : col(sf::Color::Black) { set_seed(); }
    Pixel(sf::Color col) : col(col) { set_seed(); }
    // Getter / Setter
    sf::Color getCol() const
        { return this->col; }
    void setCol(sf::Color color)
        { this->col = color; }
    // Friend
    friend std::ostream& operator<<(std::ostream& o, Pixel& p) {
        o << static_cast<int>(p.getCol().r)
            << static_cast<int>(p.getCol().g)
            << static_cast<int>(p.getCol().b)
            << ' ';
        return o;
    }
    // Static
    static void set_seed() {
        srand(time(0));
    }
    static void switch_pixel(int x1, int y1, int x2, int y2) {
        std::swap(grid[y1][x1], grid[y2][x2]);
        grid[y1][x1]->render(x1, y1);
        grid[y2][x2]->render(x2, y2);
    }
    static bool is_empty(int x, int y) {
        return grid[y][x]->getCol() == sf::Color::Black;
    }
    static void processPixels() {
        for(int i = grid.size()-1; i >= 0; --i)
            for(int j = 0; j < grid.size(); ++j) {
                grid[i][j]->process(j, i);
        }
    }
    // Universal / Utility
    void render(int x, int y) {
        image.setPixel(x, y, getCol() );
    }
    float random() {
        return (float)(rand()) / (float)(RAND_MAX); 
    }
    // Derivable
    virtual void process(int x, int y) {}
};