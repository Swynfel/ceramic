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
    friend std::ostream& operator<<(std::ostream& os, Action tile);
    friend std::istream& operator>>(std::istream& is, Action& tile);
    std::string str() const;
    std::string repr() const;
};

#endif //ACTION_HPP
