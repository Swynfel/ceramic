#include "tile.hpp"

#include <sstream>

Tile::Tile()
  : value(TILE_TYPES) {}

Tile::Tile(ushort value, bool check)
  : value(value) {
    if (check && value >= TILE_TYPES) {
        throw std::invalid_argument("Tile color was too big (" + std::to_string(value) + ")");
    }
}

Tile::Tile(ushort value)
  : Tile(value, true) {}

Tile::Tile(const Tile& tile)
  : value(tile.value) {}

const Tile Tile::NONE = Tile();

bool
operator==(Tile left, Tile right) {
    return left.value == right.value;
}

bool
operator!=(Tile left, Tile right) {
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

constexpr const int INT_OF_CHAR_A = 'A';
constexpr const int INT_OF_CHAR_a = 'a';

char
Tile::value_to_letter(ushort v) {
    return v != TILE_TYPES ? INT_OF_CHAR_A + v : ' ';
}

ushort
Tile::letter_to_value(char l) {
    if (l == ' ' || l == '-' || l == '_') {
        return TILE_TYPES;
    }
    if (l >= INT_OF_CHAR_A && l < INT_OF_CHAR_A + TILE_TYPES) {
        return l - INT_OF_CHAR_A;
    }
    if (l >= INT_OF_CHAR_a && l < INT_OF_CHAR_a + TILE_TYPES) {
        return l - INT_OF_CHAR_a;
    }
    throw std::invalid_argument("Invalid character to convert into Tile value: " + l);
}

Tile
Tile::from_letter(char l) {
    return Tile(letter_to_value(l), false);
}

char
Tile::letter() const {
    return value_to_letter(value);
}

// Reading

ostream&
operator<<(ostream& os, const Tile tile) {
    return os << '<' << tile.letter() << '>';
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