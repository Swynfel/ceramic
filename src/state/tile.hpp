#ifndef TILE_HPP
#define TILE_HPP

#include "../global.hpp"
#include <memory>
#include <string>

using namespace std;

struct Tile {
private:
    ushort value;

public:
    Tile();
    Tile(ushort value);
    Tile(const Tile& tile);

    static const Tile NONE;

    friend bool operator==(const Tile& left, const Tile& right);
    friend bool operator!=(const Tile& left, const Tile& right);
    bool is_none() const;
    operator int() const;

    // Reading
    friend ostream& operator<<(ostream& os, const Tile& tile);
    string str() const;
    string repr() const;
};

#endif //TILE_HPP