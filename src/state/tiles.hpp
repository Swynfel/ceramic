#ifndef TILES_HPP
#define TILES_HPP

#include <array>
#include <memory>
#include <string>
#include <vector>

#include "../global.hpp"

using namespace std;

/*** Tile ***/

struct Tile {
private:
    ushort value;

public:
    Tile();
    Tile(ushort value);
    Tile(const Tile& tile);

    static const Tile NONE;

    operator int() const;
    friend ostream& operator<<(ostream& os, const Tile& tiles);
    friend bool operator==(const Tile& left, const Tile& right);
    friend bool operator!=(const Tile& left, const Tile& right);
    bool is_none() const;
    string str() const;
};

/*** Tiles ***/

class Tiles {
    array<ushort, TILE_TYPES> quantities;

public:
    // Constructors
    Tiles();
    Tiles(const Tile& tile);
    Tiles(const vector<ushort>& tiles);
    Tiles(const Tiles& tiles);

    // Utils
    ushort& operator[](Tile tile);
    ushort total() const;
    constexpr ushort size() const { return TILE_TYPES; }

    friend bool operator==(const Tiles& left, const Tiles& right);
    friend bool operator!=(const Tiles& left, const Tiles& right);
    friend bool operator<=(const Tiles& left, const Tiles& right);
    friend bool operator>=(const Tiles& left, const Tiles& right);

    Tiles operator+(const Tiles& other) const;
    Tiles operator-(const Tiles& other) const;
    Tiles& operator+=(const Tiles& other);
    Tiles& operator-=(const Tiles& other);

    Tiles operator+(const Tile& tile) const;
    Tiles operator-(const Tile& tile) const;
    Tiles& operator+=(const Tile& tile);
    Tiles& operator-=(const Tile& tile);

    friend bool operator<=(const Tiles& left, const ushort& right);
    friend bool operator>=(const Tiles& left, const ushort& right);

    // Reading
    friend ostream& operator<<(ostream& os, const Tiles& tiles);

    string str() const;
    array<ushort, TILE_TYPES> get_quantities() const;

    static const Tiles ZERO;
};

#endif //TILES_HPP
