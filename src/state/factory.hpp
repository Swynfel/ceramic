#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <string>

#include "../global.hpp"
#include "tiles.hpp"

class Factory {
public:
    ushort id;
    Tiles tiles;
    // Constructor
    Factory(ushort);

    // Utils
    bool is_empty() const;
    bool has_color(Tile color) const;

    // Reading
    friend ostream& operator<<(ostream& os, const Factory& factory);
    string str() const;
    string repr() const;
};

#endif //FACTORY_HPP
