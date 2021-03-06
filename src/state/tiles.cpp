#include "tiles.hpp"

#include <algorithm>
#include <iterator>
#include <numeric>
#include <sstream>

// Constructors

Tiles::Tiles()
  : quantities() {}

Tiles::Tiles(Tile tile)
  : Tiles(tile, 1) {}

Tiles::Tiles(Tile tile, int count)
  : quantities() {
    if (bool(tile)) {
        quantities[(int)(tile)] = count;
    }
}

Tiles::Tiles(const std::vector<ushort>& tiles)
  : quantities() {
    uint index = 0;
    for (ushort q : tiles) {
        if (index > TILE_TYPES) {
            break;
        }
        quantities[index] = q;
        index++;
    }
}

Tiles::Tiles(const Tiles& tiles)
  : quantities(tiles.quantities) {}

// Utils

ushort&
Tiles::operator[](Tile tile) {
    if (!tile) {
        throw std::range_error("Can't get amount of null tiles");
    }
    return quantities[int(tile)];
}

ushort
Tiles::total() const {
    return std::accumulate(
        quantities.begin(),
        quantities.end(),
        0);
}

bool
Tiles::is_empty() const {
    return total() == 0;
}

bool
Tiles::has_color(Tile color) const {
    if (!color) {
        throw std::range_error("Can't call with null tile color");
    }
    return quantities[int(color)] > 0;
}

Tiles&
Tiles::operator+=(Tiles other) {
    for (int i = 0; i < TILE_TYPES; i++) {
        this->quantities[i] += other.quantities[i];
    }
    return *this;
}

Tiles&
Tiles::operator-=(Tiles other) {
    if (*this >= other) {
        throw std::invalid_argument("Not enough Tiles to substract");
    }
    for (int i = 0; i < TILE_TYPES; i++) {
        this->quantities[i] -= other.quantities[i];
    }
    return *this;
}

Tiles
Tiles::operator+(Tiles other) const {
    Tiles t(*this);
    t += other;
    return t;
}

Tiles
Tiles::operator-(Tiles other) const {
    Tiles t(*this);
    t -= other;
    return t;
}

bool
operator==(Tiles left, Tiles right) {
    return left.quantities == right.quantities;
}

bool
operator!=(Tiles left, Tiles right) {
    return !(left == right);
}

bool
operator<=(Tiles left, Tiles right) {
    for (int i = 0; i < TILE_TYPES; i++) {
        if (left.quantities[i] > right.quantities[i]) {
            return false;
        }
    }
    return true;
}

bool
operator>=(Tiles left, Tiles right) {
    for (int i = 0; i < TILE_TYPES; i++) {
        if (left.quantities[i] < right.quantities[i]) {
            return false;
        }
    }
    return true;
}

Tiles&
Tiles::operator+=(Tile tile) {
    if (bool(tile)) {
        this->quantities[int(tile)]++;
    }
    return *this;
}

Tiles&
Tiles::operator-=(Tile tile) {
    if (bool(tile)) {
        if (this->quantities[int(tile)] == 0) {
            throw std::invalid_argument("Not enough Tiles to substract");
        }
        this->quantities[int(tile)]--;
    }
    return *this;
}

Tiles
Tiles::operator+(Tile tile) const {
    Tiles t(*this);
    t += tile;
    return t;
}

Tiles
Tiles::operator-(Tile tile) const {
    Tiles t(*this);
    t -= tile;
    return t;
}

// Reading

std::ostream&
operator<<(std::ostream& os, Tiles tiles) {
    bool first = true;
    for (uint v : tiles.quantities) {
        if (first) {
            first = false;
            os << '[';
        } else {
            os << ',';
        }
        os << v;
    }
    if (first) {
        os << '[';
    }
    os << ']';
    return os;
}

std::string
Tiles::str() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

std::string
Tiles::letter_str() const {
    std::ostringstream os;
    for (ushort tile_value = 0; tile_value < TILE_TYPES; tile_value++) {
        Tile tile = Tile(tile_value);
        for (int i = 0; i < quantities[int(tile)]; i++) {
            os << tile.letter();
        }
    }
    return os.str();
}

std::string
Tiles::repr() const {
    std::ostringstream os;
    os << "Tiles:" << *this;
    return os.str();
}

const std::array<ushort, TILE_TYPES>&
Tiles::get_quantities() const {
    return quantities;
}

void
Tiles::set_quantities(std::array<ushort, TILE_TYPES> _quantities) {
    quantities = _quantities;
}

const Tiles Tiles::ZERO = Tiles();