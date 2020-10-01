#ifndef TILES_HPP
#define TILES_HPP

#include <array>
#include <string>
#include <vector>

#include "global.hpp"
#include "tile.hpp"

class Tiles {
protected:
    std::array<ushort, TILE_TYPES> quantities;

public:
    // Constructors
    Tiles();
    Tiles(Tile tile);
    Tiles(Tile tile, int count);
    Tiles(const std::vector<ushort>& tiles);
    Tiles(const Tiles& tiles);
    virtual ~Tiles() = default;

    // Utils
    ushort& operator[](Tile tile);
    ushort total() const;
    constexpr ushort size() const { return TILE_TYPES; }
    bool is_empty() const;
    bool has_color(const Tile color) const;

    friend bool operator==(Tiles left, Tiles right);
    friend bool operator!=(Tiles left, Tiles right);
    friend bool operator<=(Tiles left, Tiles right);
    friend bool operator>=(Tiles left, Tiles right);

    Tiles operator+(Tiles other) const;
    Tiles operator-(Tiles other) const;
    Tiles& operator+=(Tiles other);
    Tiles& operator-=(Tiles other);

    Tiles operator+(Tile tile) const;
    Tiles operator-(Tile tile) const;
    Tiles& operator+=(Tile tile);
    Tiles& operator-=(Tile tile);

    friend bool operator<=(Tiles left, ushort right);
    friend bool operator>=(Tiles left, ushort right);

    // Reading
    friend std::ostream& operator<<(std::ostream& os, Tiles tiles);
    std::string virtual str() const;
    std::string virtual letter_str() const;
    std::string virtual repr() const;

    const std::array<ushort, TILE_TYPES>& get_quantities() const;
    void set_quantities(std::array<ushort, TILE_TYPES> quantities);

    static const Tiles ZERO;
};

#endif //TILES_HPP
