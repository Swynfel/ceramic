#include "center.hpp"

#include <sstream>

// Constructor

Center::Center()
  : Tiles()
  , first_token(false) {}

void
Center::set_tiles(const Tiles tiles) {
    quantities = tiles.get_quantities();
}

// Reading

ostream&
operator<<(ostream& os, const Center& center) {
    os << "<" << center.letter_str() << (center.first_token ? "<+>" : "") << ">";
    return os;
}

string
Center::str() const {
    ostringstream os;
    os << *this;
    return os.str();
}

string
Center::repr() const {
    ostringstream os;
    os << "<Center:" << (Tiles)(*this) << (first_token ? "<+>" : "") << ">";
    return os.str();
}