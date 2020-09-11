#ifndef TILE_HPP
#define TILE_HPP

#include "global.hpp"
#include <memory>
#include <string>

using namespace std;

struct Tile {
private:
    ushort value;
    Tile(ushort value, bool check);

public:
    Tile();
    Tile(ushort value);
    Tile(const Tile& tile);

    static const Tile NONE;

    friend bool operator==(Tile left, Tile right);
    friend bool operator!=(Tile left, Tile right);
    operator int() const;
    operator bool() const;
    operator ushort() const;

    static char value_to_letter(ushort v);
    static ushort letter_to_value(char c);
    static Tile from_letter(char c);
    char letter() const;

    // Reading
    friend ostream& operator<<(ostream& os, Tile tile);
    string str() const;
    string repr() const;
};

#endif //TILE_HPP