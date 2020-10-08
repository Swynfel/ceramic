#include "factory.hpp"

#include <sstream>

// Constructor

Factory::Factory(ushort id)
  : id(id)
  , tiles() {}

Factory::Factory(ushort id, Tiles tiles)
  : id(id)
  , tiles(tiles) {}


bool
operator==(const Factory& left, const Factory& right) {
    return left.tiles == right.tiles;
}

bool
operator!=(const Factory& left, const Factory& right) {
    return !(left == right);
}

// Reading

std::ostream&
operator<<(std::ostream& os, const Factory& factory) {
    os << "<#" << factory.id << ":" << factory.tiles.letter_str() << ">";
    return os;
}

std::string
Factory::str() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

std::string
Factory::repr() const {
    std::ostringstream os;
    os << "<Factory#" << id << ":" << tiles << ">";
    return os.str();
}