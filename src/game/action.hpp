#ifndef ACTION_HPP
#define ACTION_HPP

#include "global.hpp"
#include "state/tiles.hpp"

struct Action {
    ushort pick;  // id of Factory to pick
    Tile color;   // color to take in Factory
    ushort place; // line on Pyramid to place

    friend bool operator==(Action left, Action right);
    friend bool operator!=(Action left, Action right);

    // Reading
    friend ostream& operator<<(ostream& os, Action tile);
    string str() const;
    string repr() const;
};

#endif //ACTION_HPP
