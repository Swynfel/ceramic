#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <string>

#include "../global.hpp"
#include "tiles.hpp"

class Factory : public Tiles {
public:
    ushort id;
    // Constructor
    Factory(ushort);

    void set_tiles(const Tiles tiles);

    // Reading
    friend ostream& operator<<(ostream& os, const Factory& factory);
    string str() const;
    string repr() const;
};

#endif //FACTORY_HPP
