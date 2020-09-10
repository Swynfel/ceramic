#ifndef TILES_HPP
#define TILES_HPP

#include <array>
#include <string>
#include <vector>

#include "../global.hpp"
#include "tile.hpp"

using namespace std;

class Tiles {
protected:
    array<ushort, TILE_TYPES> quantities;

public:
    // Constructors
    Tiles();
    Tiles(Tile tile);
    Tiles(Tile tile, int count);
    Tiles(const vector<ushort>& tiles);
    Tiles(const Tiles& tiles);

    // Utils
    ushort& operator[](Tile tile);
    ushort total() const;
    constexpr ushort size() const { return TILE_TYPES; }
    bool is_empty() const;
    bool has_color(const Tile color) const;

    friend bool operator==(Tiles left, Tiles right);
    friend bool operator!=(Tiles left, Tiles right);
    friend bool operator<=(Tiles left, Tiles right);
    friend bool operator>=(Tiles left, Tiles right);

    Tiles operator+(const Tiles other) const;
    Tiles operator-(const Tiles other) const;
    Tiles operator+=(const Tiles other);
    Tiles operator-=(const Tiles other);

    Tiles operator+(const Tile tile) const;
    Tiles operator-(const Tile tile) const;
    Tiles operator+=(const Tile tile);
    Tiles operator-=(const Tile tile);

    friend bool operator<=(const Tiles left, const ushort& right);
    friend bool operator>=(const Tiles left, const ushort& right);

    // Reading
    friend ostream& operator<<(ostream& os, const Tiles tiles);
    string virtual str() const;
    string virtual repr() const;

    array<ushort, TILE_TYPES> get_quantities() const;
    void set_quantities(array<ushort, TILE_TYPES> quantities);

    static const Tiles ZERO;
};

#endif //TILES_HPP
