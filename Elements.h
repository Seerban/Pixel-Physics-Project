#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include "Pixel.h"
#include "Dust.h"
#include "Gas.h"
#include "Liquid.h"

std::unordered_map< std::string, Pixel* > elements = {
    { "water",  new Liquid(sf::Color(25,55,235,255)) },
    { "earth",  new Pixel(sf::Color(155, 55, 35)) },
    { "steam",  new Gas(sf::Color(155, 155, 165)) },
    { "sand",   new Dust(sf::Color(225, 225, 155)) }
};