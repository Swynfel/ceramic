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

    // Utils
    Tiles get_tiles() const;
};

#endif //FACTORY_HPP
