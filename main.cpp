#include "Grid.h"

const int SIZE = 80; //multiple of 8
const int SCALE = 6;
const int FPS = 30;

void show_controls() {
    std::cout<<"1-9 - Brush Size\n";
    std::vector< std::pair< char, std::string > > list;
    for(auto it=key_to_elem.begin(); it!=key_to_elem.end(); ++it)
        list.emplace_back( it->first, it->second );
    std::sort( list.begin(), list.end(), [](std::pair<char, std::string> a, std::pair<char, std::string> b) {return a.first < b.first;});
    for(auto it=list.begin(); it!=list.end(); ++it) std::cout<<it->first<<" - "<<it->second<<std::endl;
}

int main() {
    show_controls();
    Grid grid(SIZE, SCALE, FPS);
    grid.start();
}
