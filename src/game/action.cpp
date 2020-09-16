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

istream&
operator>>(istream& is, Action& action) {
    istream::sentry sentry(is);
    if (!sentry) {
        is.setstate(std::ios_base::failbit);
        return is;
    }
    // Pull characters
    if (!is.good()) {
        throw invalid_argument("Not enough characters for Action");
    }
    char pick_char = is.get();
    if (!is.good()) {
        throw invalid_argument("Not enough characters for Action");
    }
    char color_char = is.get();
    if (!is.good()) {
        throw invalid_argument("Not enough characters for Action");
    }
    char place_char = is.get();
    // Convert characters
    int pick = pick_char - '0';
    if (pick < 0 || pick >= 10) {
        throw invalid_argument("Wrong character for pick: " + pick_char);
    }
    Tile color = Tile::from_letter(color_char);
    int place = place_char - '0';
    if (place < 0 || place >= 10) {
        throw invalid_argument("Wrong character for place: " + place_char);
    }
    // Set
    action.pick = pick;
    action.color = color;
    action.place = place;
    return is;
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