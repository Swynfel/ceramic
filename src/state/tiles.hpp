#ifndef TILES_HPP
#define TILES_HPP

#include <array>
#include <string>
#include <vector>

#include "global.hpp"
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

    Tiles operator+(Tiles other) const;
    Tiles operator-(Tiles other) const;
    Tiles& operator+=(Tiles other);
    Tiles& operator-=(Tiles other);

    Tiles operator+(Tile tile) const;
    Tiles operator-(Tile tile) const;
    Tiles& operator+=(Tile tile);
    Tiles& operator-=(Tile tile);

    friend bool operator<=(Tiles left, ushort right);
    friend bool operator>=(Tiles left, ushort right);

    // Reading
    friend ostream& operator<<(ostream& os, Tiles tiles);
    string virtual str() const;
    string virtual letter_str() const;
    string virtual repr() const;

    array<ushort, TILE_TYPES> get_quantities() const;
    void set_quantities(array<ushort, TILE_TYPES> quantities);

    static const Tiles ZERO;
};

#endif //TILES_HPP
