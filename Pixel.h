#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream> // debug
#include <utility>
#include "Elements.h"

class Pixel {
    std::string elem;
    sf::Color col;
    bool processed = false;
public:
    static std::vector<std::vector< Pixel* >>  grid;
    static sf::Image image;
    static sf::Texture texture;
    static sf::Sprite sprite;

    // Constructor
    Pixel() : col(sf::Color::Black)  {}
    Pixel(std::string elem, sf::Color col) : elem(elem), col(col) {}

    // Getter / Setter
    sf::Color getCol() const
        { return this->col; }
    void setCol(sf::Color color)
        { this->col = color; }
    bool getProcessed() const {
        return this->processed;
    }
    void setProcessed(bool b) {
        this->processed = b;
    }
    std::string getElem() const {
        return this->elem.empty()? "empty" : this->elem;
    }

    // Friend
    friend std::ostream& operator<<(std::ostream& o, Pixel& p) {
        o << static_cast<int>(p.getCol().r)
            << static_cast<int>(p.getCol().g)
            << static_cast<int>(p.getCol().b)
            << ' ';
        return o;
    }

    // Static
    static bool inBounds(int x, int y) {
        return !( x < 0 || y < 0 || x >= grid.size() || y >= grid.size() );
    }
    static void set_seed() {
        srand(time(0));
    }
    static void switch_pixel(int x1, int y1, int x2, int y2) {
        std::swap(grid[y1][x1], grid[y2][x2]);
        grid[y1][x1]->render(x1, y1);
        grid[y2][x2]->render(x2, y2);
    }
    static bool is_empty(int x, int y) {
        return grid[y][x]->getElem() == "empty";
    }
    static bool tryMove(int x, int y, int x2, int y2, bool upward = false) {
        if( inBounds(x2, y2) ) {
            if( is_empty(x2, y2) ) {
                switch_pixel(x, y, x2, y2);
                return true;
            }
            if( upward )
                if( elems::density[ grid[y2][x2]->getElem() ] > elems::density[ grid[y][x]->getElem() ] and elems::density[ grid[y2][x2]->getElem() ] <= 0) {
                    switch_pixel(x, y, x2, y2);
                    return true;
                }
                else return false;
            else
                if( elems::density[ grid[y2][x2]->getElem() ] < elems::density[ grid[y][x]->getElem() ] ) {
                    switch_pixel(x, y, x2, y2);
                    return true;
                }
            }
        return false;
    }
    static void setPixel(int x, int y, std::string p) {
        delete grid[y][x];
        Pixel::grid[y][x] = elems::elements[p]->clone();
        Pixel::grid[y][x]->randomize_colors();
        Pixel::grid[y][x]->render(x,y);
    }
    static void processPixels() {
        for(int i = grid.size()-1; i >= 0; --i)
            for(int j = 0; j < grid.size(); ++j) {
                if( grid[i][j]->getProcessed() ) continue;
                grid[i][j]->setProcessed(true);
                grid[i][j]->process(j, i);
        }
        for(int i = grid.size()-1; i >= 0; --i)
            for(int j = 0; j < grid.size(); ++j)
                grid[i][j]->setProcessed(false);
    }
    static float random() {
        return (float)(rand()) / (float)(RAND_MAX); 
    }
    static int random_color(int x) {
        float factor = 0.9f + static_cast<float>(random()) * 0.2f;
        int temp =  static_cast<sf::Uint8>(std::min(255, int(x * factor)));
        return temp;
    }

    // Universal / Utility
    void render(int x, int y) {
        image.setPixel(x, y, getCol() );
    }
    bool transform(int x, int y, std::string elem) {
        auto it = elems::reaction[ grid[y][x]->getElem() ].find(elem);
        //std::cout<<"CHECKING " << grid[y][x]->getElem() << " WITH " << elem << std::endl;
        if( it != elems::reaction[ grid[y][x]->getElem() ].end() ) {
            float chance = elems::reaction[ grid[y][x]->getElem() ][elem].second;
            std::string element = elems::reaction[ grid[y][x]->getElem() ][elem].first;
            if( random() <= chance ) {
                //std::cout << "REACTING " <<  grid[y][x]->getElem() << " WITH " << elem << std::endl;
                setPixel(x, y, elems::reaction[ grid[y][x]->getElem() ][elem].first );
                return true;
            }
        }
        return false;
    }
    void reaction(int x, int y) {
        if( is_empty(x, y) ) return;
        int offsets[] {1, 0,   -1, 0,   0, 1,   0, -1};
        for(int i = 0; i < 8; i+=2) {
            if( inBounds(x+offsets[i], y+offsets[i+1]) ) {
                //std::cout<< this->getElem() << ' ' << grid[ y+offsets[i+1] ][ x+offsets[i] ]->getElem() << std::endl;
                // auxilliary element used for a mutual eaction in case neighbor gets transformed
                std::string temp_elem = grid[y][x]->getElem();
                bool trans = transform(x, y, grid[ y+offsets[i+1] ][ x+offsets[i] ]->getElem());
                if( trans || is_empty(x+offsets[i], y+offsets[i+1])) transform(x+offsets[i], y+offsets[i+1], temp_elem);
            }
        }
    }
    void process(int x, int y) {
        //f(grid[y][x]->getElem() == "sand") std::cout << static_cast<int>(grid[y][x]->getCol().r) << std::endl;
        reaction(x, y);
        movement(x, y);
    }
    void randomize_colors() {
        setCol( sf::Color(random_color(this->col.r), random_color(this->col.g), random_color(this->col.b)) );
    }

    // Derivable
    virtual void movement(int x, int y) {}
    virtual Pixel* clone() const { return new Pixel(*this); }
};