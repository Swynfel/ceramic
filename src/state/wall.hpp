#ifndef WALL_HPP
#define WALL_HPP

#include <vector>

#include "../global.hpp"
#include "rules.hpp"
#include "tiles.hpp"

class Wall {
private:
    const ushort size;
    vector<bool> placed;

public:
    Wall(ushort size);
    Wall(const Rules& rule);
    Wall(const Wall& wall);

    bool is_placed_at(ushort x, ushort y) const;
    const vector<bool> get_placed() const;
    vector<vector<bool>> get_placed_array() const;
};

#endif //WALL_HPP
