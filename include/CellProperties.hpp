#ifndef CELLPROPERTIES_HPP
#define CELLPROPERTIES_HPP

#include <SFML/Graphics.hpp>
/*
    Our representation for cells and how we determine equality and visuals in the game. 
    It is made up of an Enum Type and a Color, with a default type of empty when initializing the game. 
*/

enum class CellType{
    EMPTY, // (Air)
    SAND,
    WATER,
    ROCK,
    SMOKE,
    ACID,
    GODBLOCK,
    LAVA
    // More to be implemented
};

struct Cell {
    CellType type = CellType::EMPTY;
    sf::Color color; 
};
#endif
