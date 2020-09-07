#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include <vector>

#include "../global.hpp"
#include "rules.hpp"
#include "tiles.hpp"

class Pyramid {
private:
    const ushort size;
    vector<Tile> tile_types;
    vector<ushort> tile_filled;

public:
    Pyramid(ushort size);
    Pyramid(const Rules& rule);
    Pyramid(const Pyramid& pyramid);

    bool is_filled(ushort line) const;
    vector<bool> filled() const;
};

#endif //PYRAMID_HPP
