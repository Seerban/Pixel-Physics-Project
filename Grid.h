#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include <random>
#include <utility>
#include <chrono> // debug fps info
#include "Element.h"
#include "Pixel.h"

float randf();

class Grid {
    static int size; // size of grid on both axis
    static int scale; // scale multiplier of window size

    bool even_state = false;

    static std::vector<std::vector<Pixel>> grid;
    static std::vector<std::vector<sf::Color>> debug_grid;
    static std::vector<std::vector<bool>> active_chunks;
    static sf::Image image;
    static sf::Texture texture;
    static sf::Sprite sprite;
    static sf::RenderWindow window;
    static std::vector<void(*)(int,int)> stateProcess; // inititalized in element.cpp
    public:
    // constructors
    Grid() : Grid(40) {}
    Grid(int size, int scale = 8, int fps = 12) {
            this->size = size;
            this->scale = scale;
            window.create( sf::VideoMode(size*scale, size*scale), "Grid" );
            grid.resize(size, std::vector<Pixel>(size));
            debug_grid.resize(size, std::vector<sf::Color>(size));
            active_chunks.resize(size/8, std::vector<bool>(size/8));
            image.create(size, size, sf::Color::Black);
            texture.loadFromImage(image);
            sprite.setTexture(texture);
            sprite.setScale(scale, scale);
            window.setFramerateLimit( fps );
        }
    // main process functions
    void start() {
        srand(time(0));
        while (window.isOpen()) {
            handleInput();

            mainProcess();
            texture.update(image); // should be replace to update only 1 pixel but newest sfml incompatible with my system

            window.clear();
            window.draw(sprite);
            window.display();
        }
    }
    void mainProcess() {
        // FPS DEBUGGING
        using clock = std::chrono::steady_clock;
        static auto last_time = clock::now();

        auto now = clock::now();
        std::chrono::duration<double> elapsed = now - last_time;
        last_time = now;

        double fps = 1.0 / elapsed.count();
        std::cout << "FPS: " << fps << std::endl;
        // START OF FUNCTION
        even_state = !even_state;
        //set all to unprocessed
        std::vector<std::vector<bool>> old_chunks = active_chunks;
        for(int i = 0; i < size; ++i)
            for(int j = 0; j < size; ++j)
                grid[i][j].setProcessed(false);
        for(int i = 0; i < size/8; ++i)
            for(int j = 0; j < size/8; ++j)
                if( active_chunks[j][i] ) setChunk(i, j, false);
        //main loop
        for(int i = size/8-1; i >= 0; --i)
            for(int j=0; j < size/8; ++j)
                if( old_chunks[i][j] ) {
                    int x2 = j*8, to=j*8+8, incr=1;
                    if( even_state ) {x2 = j*8+7; to=j*8-1; incr = -1; }

                    for( int y=i*8+7; y>=i*8; --y)
                        for( int x=x2; x!=to; x+=incr)
                            if( grid[y][x].getState() != elem::GAS && !grid[y][x].getProcessed() ) {
                                grid[y][x].setProcessed(true);
                                // movement utility functions defined in grid.cp
                                reactionProcess(x, y);
                                (stateProcess[ grid[y][x].getState() ])(x, y);
                            }
                    for( int y=i*8; y<i*8+8; ++y)
                        for( int x=x2; x!=to; x+=incr)
                            if( grid[y][x].getState() == elem::GAS && !grid[y][x].getProcessed() ) {
                                grid[y][x].setProcessed(true);
                                // movement utility functions defined in grid.cp
                                reactionProcess(x, y);
                                (stateProcess[ grid[y][x].getState() ])(x, y);
                            }
                }
    }
    void mainProcessOLD() {
        // FPS DEBUGGING
        using clock = std::chrono::steady_clock;
        static auto last_time = clock::now();

        auto now = clock::now();
        std::chrono::duration<double> elapsed = now - last_time;
        last_time = now;

        double fps = 1.0 / elapsed.count();
        std::cout << "FPS: " << fps << std::endl;
        // alternate left-to-right every frame
        int j, fin, incr;
        if( even_state ) { j = 0; fin = size; incr = 1; }
        else { j = size-1; fin = -1; incr = -1; }
        even_state = !even_state;
        // bottom-to-top loop for processing liquids/dusts
        for(int i = size-1; i >= 0; --i) {
            for( int j2 = j; j2 != fin; j2 += incr )
                if( grid[i][j2].getState() != elem::GAS && !grid[i][j2].getProcessed() ) {
                    grid[i][j2].setProcessed(true);
                    // movement utility functions defined in grid.cpp
                    reactionProcess(i, j2);
                    (stateProcess[ grid[i][j2].getState() ])(j2, i);
                }
        }
        // top-to-bottom loop for processing gas
        for(int i = 0; i < size; ++i) {
            for( int j2 = j; j2 != fin; j2 += incr )
                if( grid[i][j2].getState() == elem::GAS && !grid[i][j2].getProcessed() ) {
                    grid[i][j2].setProcessed(true);
                    // movement utility functions defined in grid.cpp
                    reactionProcess(i, j2);
                    (stateProcess[ grid[i][j2].getState() ])(j2, i);
                }
        }

        for(int i = 0; i < size; ++i)
            for(int j = 0; j < size; ++j)
                grid[i][j].setProcessed(false);
    }
    void handleInput(); // defined in input.h
    void reactionProcess(int x, int y) {
        checkReaction(x, y, x+1, y);
        checkReaction(x, y, x-1, y);
        checkReaction(x, y, x, y+1);
        checkReaction(x, y, x, y-1);
    }
    // pixel grid functions 
    static bool inBounds(int x, int y) {
        return !(x<0 || y<0 || x>=size || y>=size);
    }
    static bool isEmpty(int x, int y) {
        return (grid[y][x].getElem() == ""); // empty element is ""
    }
    static std::string getElem(int x, int y) {
        return grid[y][x].getElem();
    }
    static float getDensity(int x, int y) {
        return grid[y][x].getState();
    }
    static void setPixel(int x, int y, std::string s) { // not to be confused with window.setPixel
        setChunkRegion(x/8, y/8);
        grid[y][x] = Pixel(s);
        render(x, y);
    }
    static void setDebug(int x, int y, sf::Color c = sf::Color(0, 55, 0)) {
        debug_grid[y][x] = c;
        render(x, y);
    }
    static void setChunk(int x, int y, bool b) {
        if( !inBounds(x*8+7, y*8+7) ) return;
        active_chunks[y][x] = b;
        //sf::Color c = sf::Color(0, 0, 0);
        //if( b ) c = sf::Color(0, 55, 0);
        // debug colors
        //for(int i=x*8; i<x*8+8; ++i)
        //    for(int j=y*8; j<y*8+8; ++j)
        //        setDebug(i, j, c);
    }
    static void setChunkRegion(int x, int y) {
        setChunk(x-1, y-1, true);
        setChunk(x-1, y, true);
        setChunk(x-1, y+1, true);
        setChunk(x, y-1, true);
        setChunk(x, y, true);
        setChunk(x, y+1, true);
        setChunk(x+1, y-1, true);
        setChunk(x+1, y, true);
        setChunk(x+1, y+1, true);
    }
    static void render(int x, int y) {
        image.setPixel(x, y, grid[y][x].getCol() + debug_grid[y][x] );
    }
    static void switchPixel(int x, int y, int x2, int y2) {
        std::swap( grid[y][x], grid[y2][x2] );
        setChunkRegion(x/8, y/8);
        render(x,y);
        render(x2,y2);
    }
    static void checkReaction(int x, int y, int x2, int y2) {
        if( !inBounds(x2, y2) || !inBounds(x, y) ) return;
        std::string elem1 = grid[y][x].getElem();
        std::string elem2 = grid[y2][x2].getElem();
        auto it = elem::reaction[elem1].find( elem2 );
        // check if should melt
        if( elem::list[elem2].burning && elem::melt.find(elem1) != elem::melt.end() ) {
            setPixel(x, y, elem::melt[elem1] );
            return;
        }
        // check special interaction with neighbors
        if( it != elem::reaction[elem1].end() && randf() < elem::reaction[elem1][elem2].second )
        setPixel(x, y, elem::reaction[elem1][elem2].first);
        
        //auto it2 = elem::reaction[elem2].find( elem1 );
        //if( elem::list[elem1].burning && elem::melt.find(elem2) != elem::melt.end() ) {
        //    setPixel(x, y, elem::melt[elem2] );
        //    return;
        //}
        //if( it2 != elem::reaction[elem2].end() && elem::reaction[elem2][elem1].second ) {
        //    setPixel(x, y, elem::reaction[elem2][elem1].first );
        //}
    }
};