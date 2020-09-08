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
    Wall(const ushort size);
    Wall(const Rules& rule);
    Wall(const Wall& wall);

    void clear();

    bool is_placed_at(ushort x, ushort y) const;
    const vector<bool> get_placed() const;
    vector<vector<bool>> get_placed_array() const;

    // Reading
    void stream_line(ostream& os, const ushort line, bool brackets) const;
    friend ostream& operator<<(ostream& os, const Wall& wall);
    string str() const;
    string repr() const;
};

#endif //WALL_HPP
