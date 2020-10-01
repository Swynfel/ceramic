#include "player.hpp"

#include "game.hpp"
#include <iostream>
#include <sstream>

bool
Player::check_rules(const Rules& /*rules*/) const {
    return true;
}

std::shared_ptr<Observer>
Player::observer() {
    return nullptr;
}

void
Player::error(std::string message) {
    std::cout << "[PLAYER:" << player_type() << ":ERROR]" << message << std::endl;
}


// Reading

std::string
Player::player_type() const {
    return "";
}

std::ostream&
operator<<(std::ostream& os, const Player& player) {
    return os << "<Player:" << player.player_type() << ">";
}

std::string
Player::str() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

std::string
Player::repr() const {
    return str();
}