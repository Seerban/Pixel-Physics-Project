#include "Grid.h"

Grid::Grid(int size, int scale, int fps, sf::Image &img, sf::RenderWindow &win) {
    this->size = size;
    this->scale = scale;
    this->image = &img;
    this->window = &win;
    
    grid.resize(size, std::vector<Pixel>(size));
    debug_grid.resize(size, std::vector<sf::Color>(size));
    active_chunks.resize(size/CHUNK, std::vector<bool>(size/CHUNK));
}

void Grid::process() {
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
                        if( !isEmpty(x, y) && grid[y][x].getState() != state::GAS && !grid[y][x].getProcessed() ) {
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
                        if( !isEmpty(x, y) && grid[y][x].getState() == state::GAS && !grid[y][x].getProcessed() ) {
                            processPixel(x, y);
                        }
            }
}
void Grid::processPixel(int x, int y) {
    grid[y][x].setProcessed(true);
    reactionProcess(x, y);
    wetProcess(x, y);
    tempProcess(x, y);
    render(x, y);
    // movement utility functions defined in grid.cpp
    (state::stateProcess[ grid[y][x].getState() ])(*this, x, y);
}
void Grid::reactionProcess(int x, int y) {
    checkReaction(x, y, x+1, y);
    checkReaction(x, y, x-1, y);
    checkReaction(x, y, x, y+1);
    checkReaction(x, y, x, y-1);
}
void Grid::wetProcess(int x, int y) {
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
void Grid::tempProcess(int x, int y) {
    if( isEmpty(x, y) ) return;
    int temp = grid[y][x].getTemp();
    std::string element = grid[y][x].getElem();
    if( temp > 20 && temp < 40) {
        setTemp(x, y, 30);
        return;
    }
    setChunkRegion(x/CHUNK, y/CHUNK);
    
    if( temp <= elem::list[element].freeze_temp ) {
        std::string freeze = elem::list[element].freeze_to;
        setPixel(x, y, freeze);
        if( freeze == "" ) setTemp(x, y, 30);
        return;
    }
    if( temp >= elem::list[element].melt_temp ) {
        std::string melt = elem::list[element].melt_to;
        setPixel(x, y, melt);
        if( elem::list[element].flammable ) setTemp(x, y, elem::list[melt].temperature );
        if( melt == "" ) setTemp(x, y, 30);
        return;
    }
    int incrs[] = {1, 0, -1, 0, 0, 1, 0, -1};
    for(int i = 0; i < 8; i+=2) {
        if( !inBounds(y+incrs[i], x+incrs[i+1]) || isEmpty(x+incrs[i+1], y+incrs[i]) ) {
            setTemp( x, y, elemutil::intStep( getTemp(x, y), 30, 5) );
            continue;
        }
            
        double temp1 = grid[y][x].getTemp();
            double temp2 = grid[y+incrs[i]][x+incrs[i+1]].getTemp();
            grid[y+incrs[i]][x+incrs[i+1]].setTemp( temp1*0.2 + temp2*0.8 );
            grid[y][x].setTemp( temp1*0.8 + temp2*0.2 );
        }
    }
void Grid::checkReaction(int x, int y, int x2, int y2, bool overwrite) {
    if( !inBounds(x2, y2) || !inBounds(x, y) ) return;
    if( !overwrite && !grid[y2][x2].getProcessed() ) return; // overwrite means checking reaction again this frame
    std::string elem1 = grid[y][x].getElem();
    std::string elem2 = grid[y2][x2].getElem();
    if( elem1 == elem2 ) return;
    auto it = elem::reaction[elem1].find( elem2 );
    
    // check special interaction with neighbors
    if( it != elem::reaction[elem1].end() )
    setPixel(x, y, elem::reaction[elem1][elem2]);
    auto it2 = elem::reaction[elem2].find( elem1 );
    if( it2 != elem::reaction[elem2].end() ) {
        setPixel(x2, y2, elem::reaction[elem2][elem1] );
        grid[y2][x2].setProcessed(true);
    }
}

bool Grid::inBounds(int x, int y) {
    return !(x<0 || y<0 || x>=size || y>=size);
}
bool Grid::isEmpty(int x, int y) {
    return grid[y][x].getElem() == ""; // empty element is ""
}
std::string Grid::getElem(int x, int y) {
    return grid[y][x].getElem();
}
float Grid::getDensity(int x, int y) {
    return elem::list[this->grid[y][x].getElem()].density;
}

void Grid::setDebug(int x, int y, sf::Color c) {
    this->debug_grid[y][x] = c;
    render(x, y);
}
void Grid::setChunk(int x, int y, bool b) {
    if( !inBounds(x*CHUNK+CHUNK-1, y*CHUNK+CHUNK-1) ) return;
    active_chunks[y][x] = b;
    
    //COLORIZES CHUNKS, HEAVY PERFORMANCE IMPACT
    if( debug ) {
        sf::Color c = sf::Color(0, 0, 0);
        if( b ) c = sf::Color(0, 55, 0);
        // debug colors
        for(int i=x*CHUNK; i<x*CHUNK+CHUNK; ++i)
            for(int j=y*CHUNK; j<y*CHUNK+CHUNK; ++j)
                setDebug(i, j, c);
    }
}
void Grid::setChunkRegion(int x, int y) {
    setChunk(x, y, true);
    setChunk(x-1, y, true);
    setChunk(x+1, y, true);
    setChunk(x, y-1, true);
    setChunk(x+1, y-1, true);
    setChunk(x-1, y-1, true);
    setChunk(x, y+1, true);
    setChunk(x+1, y+1, true);
    setChunk(x-1, y+1, true);
}
void Grid::clearDebug() {
    for(int i = 0; i < size; ++i)
        for(int j = 0; j < size; ++j) {
            debug_grid[i][j] = sf::Color(0, 0, 0, 0);
            render(j, i);
        }
}
void Grid::render(int x, int y) {
    sf::Color col = grid[y][x].getCol();
    int addR = getTemp(x, y) / 100;
    col.r += std::min( 255-col.r, 18*addR );
    if( getTemp(x, y) > 1000 ) {
        int addW = ( getTemp(x, y) - 1000 ) / 50;
        col.g += std::min( 255-col.g, 6*addW );
        col.b += std::min( 255-col.b, 6*addW );
    }
    image->setPixel(x, y, col + debug_grid[y][x] );
}

void Grid::setPixel(int x, int y, std::string s, bool override) { // not to be confused with window->setPixel
    setChunkRegion(x/CHUNK, y/CHUNK);
    int oldtemp = getTemp(x, y);
    float oldwet = elem::list[s].wet;
    if( override ) { 
        oldtemp = elem::list[s].temperature;
        //oldwet = elem::list[s].wet;
    }
    grid[y][x] = Pixel(s);
    grid[y][x].setTemp(oldtemp);
    grid[y][x].setWet(oldwet);
    render(x, y);
}
double Grid::getTemp(int x, int y) {
    return grid[y][x].getTemp();
}
void Grid::setTemp(int x, int y, int val) {
    grid[y][x].setTemp(val);
}
float Grid::getWet(int x, int y) {
    return grid[y][x].getWet();
}
void Grid::setWet(int x, int y, int val) {
    grid[y][x].setWet(val);
}
void Grid::switchPixel(int x, int y, int x2, int y2) {
    std::swap( grid[y][x], grid[y2][x2] );
    setChunkRegion(x2/CHUNK, y2/CHUNK);
    render(x,y);
    render(x2,y2);
}
sf::Color Grid::getCol(int x, int y) {
    return grid[y][x].getCol();
}