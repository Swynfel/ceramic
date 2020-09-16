#include "factory.hpp"

#include <sstream>

// Constructor

Factory::Factory(ushort _id)
  : Tiles()
  , id(_id) {}

void
Factory::set_tiles(const Tiles tiles) {
    quantities = tiles.get_quantities();
}

// Reading

ostream&
operator<<(ostream& os, const Factory& factory) {
    os << "<#" << factory.id << ":" << factory.letter_str() << ">";
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
    os << "<Factory#" << id << ":" << (Tiles)(*this) << ">";
    return os.str();
}