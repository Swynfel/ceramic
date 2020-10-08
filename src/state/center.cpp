#include "center.hpp"

#include <sstream>

// Constructor

Center::Center()
  : tiles()
  , first_token(false) {}

Center::Center(Tiles tiles, bool first_token)
  : tiles(tiles)
  , first_token(first_token) {}

// Reading

std::ostream&
operator<<(std::ostream& os, const Center& center) {
    os << "<" << center.tiles.letter_str() << (center.first_token ? "<+>" : "") << ">";
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
    os << "<Center:" << tiles << (first_token ? "<+>" : "") << ">";
    return os.str();
}