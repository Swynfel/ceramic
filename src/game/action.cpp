#include "action.hpp"

#include <sstream>

bool
operator==(Action left, Action right) {
    return left.pick == right.pick &&
           left.color == right.color &&
           left.place == right.place;
}
bool
operator!=(Action left, Action right) {
    return !(left == right);
}


ostream&
operator<<(ostream& os, Action action) {
    return os << '<' << action.pick << action.color.letter() << action.place << '>';
}

string
Action::str() const {
    ostringstream os;
    os << *this;
    return os.str();
}

string
Action::repr() const {
    return str();
}