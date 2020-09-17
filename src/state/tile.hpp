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
    explicit operator int() const;
    explicit operator bool() const;
    explicit operator ushort() const;

    static char value_to_letter(ushort value);
    static char value_to_lc_letter(ushort value);
    static ushort letter_to_value(char letter);
    static Tile from_letter(char letter);
    char letter() const;
    char lc_letter() const;

    // Reading
    friend ostream& operator<<(ostream& os, Tile tile);
    string str() const;
    string repr() const;
};

#endif //TILE_HPP