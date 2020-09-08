#include "factory.hpp"

#include <sstream>

// Constructor

Factory::Factory(ushort _id)
  : id(_id)
  , tiles() {}

// Utils

bool
Factory::is_empty() const {
    return tiles.is_empty();
}

bool
Factory::has_color(Tile color) const {
    return tiles.has_color(color);
}

// Reading

ostream&
operator<<(ostream& os, const Factory& factory) {
    os << "<#" << factory.id << ":" << factory.tiles << ">";
    return os;
}

string
Factory::str() const {
    ostringstream os;
    os << *this;
    return os.str();
}

string
Factory::repr() const {
    ostringstream os;
    os << "<Factory#" << id << ":" << tiles << ">";
    return os.str();
}