#include "action.hpp"

#include <iostream>
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
    string opt;
    std::getline(std::cin, opt);
    if (opt.size() != 3) {
        throw invalid_argument("Wrong number of characters for Action");
    }
    char pick_char = opt[0];
    char color_char = opt[1];
    char place_char = opt[2];
    // Convert characters
    int pick = opt[0] - '0';
    if (pick < 0 || pick >= 10) {
        throw invalid_argument(string("Wrong character for pick: ") + pick_char);
    }
    Tile color = Tile::from_letter(color_char);
    int place = place_char - '0';
    if (place < 0 || place >= 10) {
        throw invalid_argument(string("Wrong character for place: ") + place_char);
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