#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "../global.hpp"
#include "tiles.hpp"

class Factory {
public:
    ushort id;
    Tiles tiles;
    // Constructor
    Factory(ushort);
};

#endif //FACTORY_HPP
