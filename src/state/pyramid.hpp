#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include <vector>

#include "../global.hpp"
#include "../rules/rules.hpp"
#include "tiles.hpp"

class Pyramid {
private:
    const ushort size;
    vector<Tile> tile_types;
    vector<ushort> tile_filled;

    void assert_line(const ushort line) const;

public:
    Pyramid(const ushort size);
    Pyramid(const std::shared_ptr<Rules>& rule);
    Pyramid(const Pyramid& pyramid);

    void clear();
    void clear_line(ushort line);
    void set_line(ushort line, ushort amount, Tile color);

    bool is_filled(ushort line) const;
    bool is_empty(ushort line) const;
    ushort amount(ushort line) const;
    ushort amount_remaining(ushort line) const;
    Tile color(ushort line) const;
    vector<bool> filled() const;

    // Reading
    void stream_line(ostream& os, const ushort line, bool brackets) const;
    friend ostream& operator<<(ostream& os, const Pyramid& pyramid);
    string str() const;
    string repr() const;
};

#endif //PYRAMID_HPP
