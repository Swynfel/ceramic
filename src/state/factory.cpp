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

std::ostream&
operator<<(std::ostream& os, const Factory& factory) {
    os << "<#" << factory.id << ":" << factory.letter_str() << ">";
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
    os << "<Factory#" << id << ":" << (Tiles)(*this) << ">";
    return os.str();
}