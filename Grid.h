#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include <random>
#include <utility>
#include <cmath> // fabs
#include <chrono> // debug fps info
#include <string>
#include "Element.h"
#include "Pixel.h"

float randf();

const int CHUNK = 4;
extern std::unordered_map<char, std::string> key_to_elem;

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

    sf::Font font;
    static sf::Text cornerText;
    static sf::Text cornerText2;

    static std::vector<void(*)(int,int)> stateProcess; // inititalized in element.cpp
    public:
    // constructors
    Grid() : Grid(40) {}
    Grid(int size, int scale = 8, int fps = 12) {
            this->size = size;
            this->scale = scale;
            window.create( sf::VideoMode(size*scale, size*scale), "Grid" );
            window.setFramerateLimit( fps );
            
            image.create(size, size, sf::Color::Black);
            texture.loadFromImage(image);
            sprite.setTexture(texture);
            sprite.setScale(scale, scale);
            
            grid.resize(size, std::vector<Pixel>(size));
            debug_grid.resize(size, std::vector<sf::Color>(size));
            active_chunks.resize(size/CHUNK, std::vector<bool>(size/CHUNK));

            font.loadFromFile( "Roboto-Bold.ttf" );
            cornerText.setFont(font);
            cornerText.setCharacterSize(12);
            cornerText.setFillColor( sf::Color(255, 255, 255) );
            cornerText.setString( "Hello World" );
            cornerText2.setFont(font);
            cornerText2.setCharacterSize(12);
            cornerText2.setFillColor( sf::Color(255, 255, 255) );
            cornerText2.setString( "abcdef");
            cornerText2.setPosition( sf::Vector2f(0, size*scale - 12) );
        }
    // main process functions
    void start() {
        srand(time(0));
        while (window.isOpen()) {
            handleInput();

            mainProcess();
            texture.update(image);
            window.clear();
            window.draw(sprite);

            updateText();

            window.draw(cornerText);
            window.draw(cornerText2);

            window.display();
        }
    }
    void mainProcess() {
        fps(); // only shows digits and erases
        even_state = !even_state;
        //set all to unprocessed
        std::vector<std::vector<bool>> old_chunks = active_chunks;
        for(int i = 0; i < size; ++i)
            for(int j = 0; j < size; ++j)
                grid[i][j].setProcessed(false);
        for(int i = 0; i < size/CHUNK; ++i)
            for(int j = 0; j < size/CHUNK; ++j)
                if( active_chunks[j][i] ) setChunk(i, j, false); // if any element updates the chunk it will be activated
        //main loop
        for(int i = size/CHUNK-1; i >= 0; --i)
            for(int j=0; j < size/CHUNK; ++j)
                if( old_chunks[i][j] ) {
                    int x2 = j*CHUNK, to=j*CHUNK+CHUNK, incr=1;
                    if( even_state ) {x2 = j*CHUNK+CHUNK-1; to=j*CHUNK-1; incr = -1; }
                    for( int y=i*CHUNK+CHUNK-1; y>=i*CHUNK; --y)
                        for( int x=x2; x!=to; x+=incr)
                            if( !isEmpty(x, y) && grid[y][x].getState() != elem::GAS && !grid[y][x].getProcessed() ) {
                                processPixel(x, y);
                            }
                }
        //gases loop
        for(int i = 0; i < size/CHUNK; ++i)
            for(int j=0; j < size/CHUNK; ++j)
                if( old_chunks[i][j] ) {
                    int x2 = j*CHUNK, to=j*CHUNK+CHUNK, incr=1;
                    if( even_state ) {x2 = j*CHUNK+CHUNK-1; to=j*CHUNK-1; incr = -1; }
                    for( int y=i*CHUNK; y<i*CHUNK+CHUNK; ++y)
                        for( int x=x2; x!=to; x+=incr)
                            if( !isEmpty(x, y) && grid[y][x].getState() == elem::GAS && !grid[y][x].getProcessed() ) {
                                processPixel(x, y);
                            }
                }
    }
    void processPixel(int x, int y) {
        grid[y][x].setProcessed(true);
        reactionProcess(x, y);
        wetProcess(x, y);
        tempProcess(x, y);
        render(x, y);
        // movement utility functions defined in grid.cpp
        (stateProcess[ grid[y][x].getState() ])(x, y);
    }
    void handleInput(); // defined in input.h
    void reactionProcess(int x, int y) {
        checkReaction(x, y, x+1, y);
        checkReaction(x, y, x-1, y);
        checkReaction(x, y, x, y+1);
        checkReaction(x, y, x, y-1);
    }
    void wetProcess(int x, int y) {
        float tempwet = grid[y][x].getWet();
        if( tempwet == 0 ) return;
        // wet
        if( tempwet >= 0.25 && elem::wet_to.find(grid[y][x].getElem()) != elem::wet_to.end() ) {
            setPixel(x, y, elem::wet_to.find(grid[y][x].getElem())->second );
            return;
        }
        // dry
        if( tempwet <= 0.75 && grid[y][x].getElem() == "water" ) {
            setPixel(x, y, "", true);
            return; 
        }
        if( tempwet <= 0.05  && elem::dry_to.find(grid[y][x].getElem()) != elem::dry_to.end()) {
            setPixel(x, y, elem::dry_to.find(grid[y][x].getElem())->second );
            return;
        }
        int incrs[] = {1, 0, -1, 0, 0, 1, 0, -1};
        
        for(int i = 0; i < 8; i+=2) {
            if( !inBounds(y+incrs[i], x+incrs[i+1]) || isEmpty(x+incrs[i+1], y+incrs[i]) ) continue;
            if( std::fabs( grid[y][x].getWet() - grid[y+incrs[i]][x+incrs[i+1]].getWet() ) < 0.01 ) continue;
            
            if( elem::list[ grid[y + incrs[i] ][x + incrs[i+1]].getElem() ].sponge ) {
                float wetTemp = grid[y][x].getWet();
                float wetTemp2 = grid[y+incrs[i]][x+incrs[i+1]].getWet();
                grid[y+incrs[i]][x+incrs[i+1]].setWet( wetTemp/4 + wetTemp2*3/4 );
                grid[y][x].setWet( wetTemp*3/4 + wetTemp2/4 );
            }
        }
    }
    void tempProcess(int x, int y) {
        if( isEmpty(x, y) ) return;
        int temp = grid[y][x].getTemp();
        if( temp != 30 ) setChunkRegion(x/CHUNK, y/CHUNK);
        // freezes
        if( temp < 0 && elem::freeze.find(grid[y][x].getElem()) != elem::freeze.end()) {
            setPixel(x, y, elem::freeze.find(grid[y][x].getElem())->second );
            return;
        }
        // unfreeze
        if( temp > 0 && elem::unfreeze.find(grid[y][x].getElem()) != elem::unfreeze.end()) {
            setPixel(x, y, elem::unfreeze.find(grid[y][x].getElem())->second );
            return;
        }
        //if( temp > 100 && grid[y][x].getWet() > 0 ) grid[y][x].setWet(0);
        if( temp > 100 ) setWet(x, y, 0);
        // melt
        if( temp > 200  && elem::melt.find(grid[y][x].getElem()) != elem::melt.end() ) {
            if( elem::list[ grid[y][x].getElem() ].flammable )
                setPixel(x, y, elem::melt.find(grid[y][x].getElem())->second , true);
            else setPixel(x, y, elem::melt.find(grid[y][x].getElem())->second );
            return;
        }
        // stop burning
        if( temp < 600 && elem::list[ grid[y][x].getElem() ].burning ) {
            if( elem::freeze.find(grid[y][x].getElem()) != elem::freeze.end() ) {
                setPixel(x, y, elem::freeze.find(grid[y][x].getElem())->second );
            }
            else setPixel(x, y, "", true);   
            return;
        }
        // high melt point
        if( temp > 1000 && elem::hardmelt.find( grid[y][x].getElem() ) != elem::hardmelt.end() ) {
            setPixel(x, y, elem::hardmelt.find(grid[y][x].getElem())->second );
            return;
        }

        int incrs[] = {1, 0, -1, 0, 0, 1, 0, -1};

        for(int i = 0; i < 8; i+=2) {
                if( !inBounds(y+incrs[i], x+incrs[i+1]) || isEmpty(x+incrs[i+1], y+incrs[i]) ) {
                    setTemp( x, y, elem::intStep( getTemp(x, y), 30, 3) );
                    continue;
                }
                
                double temp1 = grid[y][x].getTemp();
                double temp2 = grid[y+incrs[i]][x+incrs[i+1]].getTemp();
                grid[y+incrs[i]][x+incrs[i+1]].setTemp( temp1*0.2 + temp2*0.8 );
                grid[y][x].setTemp( temp1*0.8 + temp2*0.2 );
            }
        //setTemp( x, y, elem::intStep( getTemp(x, y), 30, 1) );
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
        return elem::list[grid[y][x].getElem()].density;
    }
    
    static void setDebug(int x, int y, sf::Color c = sf::Color(0, 55, 0)) {
        debug_grid[y][x] = c;
        render(x, y);
    }
    static void setChunk(int x, int y, bool b) {
        if( !inBounds(x*CHUNK+CHUNK-1, y*CHUNK+CHUNK-1) ) return;
        active_chunks[y][x] = b;
        /*
        //COLORIZES CHUNKS, HEAVY PERFORMANCE IMPACT
        sf::Color c = sf::Color(0, 0, 0);
        if( b ) c = sf::Color(0, 55, 0);
        // debug colors
        for(int i=x*CHUNK; i<x*CHUNK+CHUNK; ++i)
            for(int j=y*CHUNK; j<y*CHUNK+CHUNK; ++j)
                setDebug(i, j, c);
        */
    }
    static void setChunkRegion(int x, int y) {
        setChunk(x, y, true);
        setChunk(x-1, y, true);
        setChunk(x+1, y, true);
        setChunk(x, y-1, true);
        setChunk(x, y+1, true);
        if( grid[y][x].getState() == elem::GAS ) {
            setChunk(x+1, y-1, true);
            setChunk(x-1, y-1, true);
        }
        else {
            setChunk(x+1, y+1, true);
            setChunk(x-1, y+1, true);
        }
    }
    static void render(int x, int y) {
        sf::Color col = grid[y][x].getCol();
        int addR = getTemp(x, y) / 50;
        col.r += std::min( 255-col.r, 12*addR );
        image.setPixel(x, y, col + debug_grid[y][x] );
    }
    
    static void setPixel(int x, int y, std::string s, bool override = false) { // not to be confused with window.setPixel
        setChunkRegion(x/CHUNK, y/CHUNK);
        int oldtemp = getTemp(x, y);
        float oldwet = getWet(x, y);
        if( override ) { 
            oldtemp = elem::list[s].temperature;
            oldwet = elem::list[s].wet;
        }
        grid[y][x] = Pixel(s);
        grid[y][x].setTemp(oldtemp);
        grid[y][x].setWet(oldwet);
        render(x, y);
    }
    static double getTemp(int x, int y) {
        return grid[y][x].getTemp();
    }
    static void setTemp(int x, int y, int val) {
        grid[y][x].setTemp(val);
    }
    static float getWet(int x, int y) {
        return grid[y][x].getWet();
    }
    static void setWet(int x, int y, int val) {
        grid[y][x].setWet(val);
    }
    static void switchPixel(int x, int y, int x2, int y2) {
        std::swap( grid[y][x], grid[y2][x2] );
        setChunkRegion(x2/CHUNK, y2/CHUNK);
        render(x,y);
        render(x2,y2);
    }
    static void checkReaction(int x, int y, int x2, int y2) {
        if( !inBounds(x2, y2) || !inBounds(x, y) ) return;
        std::string elem1 = grid[y][x].getElem();
        std::string elem2 = grid[y2][x2].getElem();
        if( elem1 == elem2 ) return;
        auto it = elem::reaction[elem1].find( elem2 );
        
        // check special interaction with neighbors
        if( it != elem::reaction[elem1].end() && randf() < elem::reaction[elem1][elem2].second )
        setPixel(x, y, elem::reaction[elem1][elem2].first);

        auto it2 = elem::reaction[elem2].find( elem1 );
        if( it2 != elem::reaction[elem2].end() && elem::reaction[elem2][elem1].second ) {
            setPixel(x2, y2, elem::reaction[elem2][elem1].first );
        }
    }

    static void updateText() {
        sf::Vector2i pos = sf::Vector2i(sf::Mouse::getPosition(window).x / scale, sf::Mouse::getPosition(window).y / scale);
        if( inBounds(pos.x, pos.y) ) {
            sf::Color col = grid[pos.y][pos.x].getCol();
            cornerText.setFillColor( sf::Color( (col.r + 200)/2, (col.g + 200)/2, (col.b + 200)/2 ) );

            int temp = grid[pos.y][pos.x].getTemp();

            cornerText.setString( getElem(pos.x, pos.y) + '\n' + std::to_string(temp) + "C" );
        }
    }

    // FPS DEBUGGING
    static void fps() {
        using clock = std::chrono::steady_clock;
        static auto last_time = clock::now();

        auto now = clock::now();
        std::chrono::duration<double> elapsed = now - last_time;
        last_time = now;

        float fps = 1.0 / elapsed.count();
        cornerText2.setString( std::to_string( int(fps+0.5) ) + " TPS" );
    }
};