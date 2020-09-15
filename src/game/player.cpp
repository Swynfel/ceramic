#include "player.hpp"

#include "game.hpp"
#include <iostream>
#include <sstream>

bool
Player::check_rules(const Rules& rules) const {
    return true;
}

std::shared_ptr<Player>
Player::ptr() {
    return shared_from_this();
}

std::shared_ptr<Observer>
Player::observer() {
    return nullptr;
}

void
Player::error(string message) {
    std::cout << "[PLAYER:" << player_type() << ":ERROR]" << message << endl;
}


// Reading

string
Player::player_type() const {
    return "";
}

ostream&
operator<<(ostream& os, const Player& player) {
    return os << "<Player:" << player.player_type() << ">";
}

string
Player::str() const {
    ostringstream os;
    os << *this;
    return os.str();
}

string
Player::repr() const {
    return str();
}