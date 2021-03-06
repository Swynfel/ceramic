#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include <vector>

#include "global.hpp"
#include "rules/rules.hpp"
#include "tiles.hpp"

class Pyramid {
private:
    const ushort size;
    std::vector<Tile> tile_types;
    std::vector<ushort> tile_filled;

    void assert_line(ushort line) const;

public:
    Pyramid(ushort size);
    Pyramid(const std::shared_ptr<const Rules>& rule);
    Pyramid(const Pyramid& pyramid);

    Pyramid& operator=(const Pyramid& other);
    friend bool operator==(const Pyramid& left, const Pyramid& right);
    friend bool operator!=(const Pyramid& left, const Pyramid& right);

    void clear();
    Tiles get_line(ushort line) const;
    void set_line(ushort line, ushort amount, Tile color);
    void clear_line(ushort line);

    bool is_filled(ushort line) const;
    bool is_empty(ushort line) const;
    ushort amount(ushort line) const;
    ushort amount_remaining(ushort line) const;
    Tile color(ushort line) const;
    bool accept_color(ushort line, Tile color) const;
    std::vector<bool> filled() const;

    // Reading
    void stream_line(std::ostream& os, ushort line, bool brackets) const;
    friend std::ostream& operator<<(std::ostream& os, const Pyramid& pyramid);
    std::string str() const;
    std::string repr() const;
};

#endif //PYRAMID_HPP
