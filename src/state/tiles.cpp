#include "tiles.hpp"

#include <algorithm>
#include <iterator>
#include <numeric>
#include <sstream>
#include <stdexcept>

// Constructors

Tiles::Tiles()
  : quantities() {}

Tiles::Tiles(Tile tile)
  : Tiles(tile, 1) {}

Tiles::Tiles(Tile tile, int count)
  : quantities() {
    quantities[(int)(tile)] = count;
}

Tiles::Tiles(const vector<ushort>& tiles)
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
    return quantities[int(tile)];
}

ushort
Tiles::total() const {
    return accumulate(
        this->quantities.begin(),
        this->quantities.end(),
        0);
}

bool
Tiles::is_empty() const {
    return total() == 0;
}

bool
Tiles::has_color(Tile color) const {
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
    this->quantities[int(tile)]++;
    return *this;
}

Tiles&
Tiles::operator-=(Tile tile) {
    if (this->quantities[int(tile)] == 0) {
        throw std::invalid_argument("Not enough Tiles to substract");
    }
    this->quantities[int(tile)]--;
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

ostream&
operator<<(ostream& os, Tiles tiles) {
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

string
Tiles::str() const {
    ostringstream os;
    os << *this;
    return os.str();
}

string
Tiles::repr() const {
    ostringstream os;
    os << "Tiles:" << *this;
    return os.str();
}

array<ushort, TILE_TYPES>
Tiles::get_quantities() const {
    return quantities;
}

void
Tiles::set_quantities(array<ushort, TILE_TYPES> _quantities) {
    quantities = _quantities;
}

const Tiles Tiles::ZERO = Tiles();