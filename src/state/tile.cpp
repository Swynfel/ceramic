#include "tile.hpp"

#include <sstream>

Tile::Tile()
  : value(TILE_TYPES) {}

Tile::Tile(ushort value)
  : value(value) {
    if (value >= TILE_TYPES) {
        throw std::invalid_argument("Tile color was too big (" + std::to_string(value) + ")");
    }
}

Tile::Tile(const Tile& tile)
  : value(tile.value) {}

const Tile Tile::NONE = Tile();

bool
operator==(const Tile left, const Tile right) {
    return left.value == right.value;
}

bool
operator!=(const Tile left, const Tile right) {
    return !(left == right);
}

Tile::operator int() const {
    return value;
}

Tile::operator bool() const {
    return value != TILE_TYPES;
}

Tile::operator ushort() const {
    return value;
}

ostream&
operator<<(ostream& os, const Tile tile) {
    if (!tile) {
        return os << "< >";
    }
    return os << '<' << (int)tile << '>';
}

string
Tile::str() const {
    ostringstream os;
    os << *this;
    return os.str();
}

string
Tile::repr() const {
    return str();
}