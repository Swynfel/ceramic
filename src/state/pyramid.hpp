#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include <vector>

#include "global.hpp"
#include "rules/rules.hpp"
#include "tiles.hpp"

class Pyramid {
private:
    const ushort size;
    vector<Tile> tile_types;
    vector<ushort> tile_filled;

    void assert_line(ushort line) const;

public:
    Pyramid(ushort size);
    Pyramid(const std::shared_ptr<Rules>& rule);
    Pyramid(const Pyramid& pyramid);

    void clear();
    Tiles get_line(ushort line);
    void set_line(ushort line, ushort amount, Tile color);
    void clear_line(ushort line);

    bool is_filled(ushort line) const;
    bool is_empty(ushort line) const;
    ushort amount(ushort line) const;
    ushort amount_remaining(ushort line) const;
    Tile color(ushort line) const;
    bool accept_color(ushort line, Tile color) const;
    vector<bool> filled() const;

    // Reading
    void stream_line(ostream& os, ushort line, bool brackets) const;
    friend ostream& operator<<(ostream& os, const Pyramid& pyramid);
    string str() const;
    string repr() const;
};

#endif //PYRAMID_HPP
