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
#ifndef SWIG
    ushort& operator[](Tile tile);
#endif
    ushort total() const;
    constexpr ushort size() const { return TILE_TYPES; }

    Tiles& operator+=(const Tiles& other);
    Tiles& operator-=(const Tiles& other);
    Tiles operator+(const Tiles& other) const;
    Tiles operator-(const Tiles& other) const;

#ifndef SWIG
    friend bool operator==(const Tiles& left, const Tiles& right);
    friend bool operator!=(const Tiles& left, const Tiles& right);
    friend bool operator<=(const Tiles& left, const Tiles& right);
    friend bool operator>=(const Tiles& left, const Tiles& right);
#endif

    Tiles& operator+=(const Tile& tile);
    Tiles& operator-=(const Tile& tile);
    Tiles operator+(const Tile& tile) const;
    Tiles operator-(const Tile& tile) const;

#ifndef SWIG
    friend bool operator<=(const Tiles& left, const ushort& right);
    friend bool operator>=(const Tiles& left, const ushort& right);
#endif

    // Reading
#ifndef SWIG
    friend ostream& operator<<(ostream& os, const Tiles& tiles);
#endif
    string str() const;
    array<ushort, TILE_TYPES> to_tuple() const;
};

#endif //TILES_HPP
