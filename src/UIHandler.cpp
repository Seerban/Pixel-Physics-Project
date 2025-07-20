#include "UIHandler.h"

UIHandler::UIHandler(int size, int scale, sf::RenderWindow &window, Grid &g) {
    this->size = size;
    this->scale = scale;
    this->font_size = 16;
    this->g = &g;
    this->window = &window;
    font.loadFromFile( "Assets/Roboto-Bold.ttf" );
    setTextProp(selection);
    setTextProp(temperature);
    setTextProp(tps);
    selection.setPosition( sf::Vector2f(0, 0) );
    temperature.setPosition( sf::Vector2f(0, font_size) );
    tps.setPosition( sf::Vector2f(0, size * scale - font_size ) );
}

void UIHandler::setTextProp(sf::Text &t) {
    t.setStyle(sf::Text::Bold);    
    t.setFont(font);
    t.setCharacterSize(12);
    t.setFillColor( sf::Color::White );
    t.setString( "Hello World" );
    t.setPosition( sf::Vector2f(0, 0) );
}
    
void UIHandler::draw() {
    sf::Vector2i mousepos = sf::Vector2i(sf::Mouse::getPosition(*window).x / scale, sf::Mouse::getPosition(*window).y / scale);
    if( g->inBounds( mousepos.x, mousepos.y ) ) {
        selection.setString( g->getElem(mousepos.x, mousepos.y) );
        sf::Color col = g->getCol(mousepos.x, mousepos.y);
        col.r = (col.r + 255) / 2;
        col.g = (col.g + 255) / 2;
        col.b = (col.b + 255) / 2;
        selection.setFillColor( col );
        temperature.setString( std::to_string( int( g->getTemp(mousepos.x, mousepos.y) ) ) + "C" );
        tps.setString( std::to_string( fps() ) );

        window->draw(selection);
        window->draw(temperature);
        window->draw(tps);
    }
}

int UIHandler::fps() {
    static auto last_time = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - last_time;
    last_time = now;
    float fps = 1.0 / elapsed.count();
    return static_cast<int>(fps + 0.5); // rounded to nearest int
}