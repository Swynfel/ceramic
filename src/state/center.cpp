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

std::ostream&
operator<<(std::ostream& os, const Center& center) {
    os << "<" << center.letter_str() << (center.first_token ? "<+>" : "") << ">";
    return os;
}

std::string
Center::str() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

std::string
Center::repr() const {
    std::ostringstream os;
    os << "<Center:" << (Tiles)(*this) << (first_token ? "<+>" : "") << ">";
    return os.str();
}