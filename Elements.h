#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

class Pixel;

namespace elems {
extern std::unordered_map< std::string, Pixel* > elements;
extern std::unordered_map< std::string, std::unordered_map<std::string, std::pair< std::string, float >>> reaction;
}