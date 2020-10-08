#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <string>

#include "global.hpp"
#include "tiles.hpp"

class Factory {
public:
    ushort id;
    Tiles tiles;

    // Constructor
    Factory(ushort id);
    Factory(ushort id, Tiles tiles);

    // Reading
    friend std::ostream& operator<<(std::ostream& os, const Factory& factory);
    std::string str() const;
    std::string repr() const;
};

#endif //FACTORY_HPP
