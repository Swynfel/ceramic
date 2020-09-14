#include "player.hpp"

#include "game.hpp"
#include <iostream>
#include <sstream>


bool
Player::join_game(Game* game, ushort _id) {
    if (joined_game != nullptr) {
        throw runtime_error("Already joined game");
    }
    if (!check_rules(*(game->get_state().get_rules()))) {
        return false;
    }
    id = _id;
    joined_game = game;
    return true;
}

void
Player::set_position(ushort _position) {
    position = _position;
}

void
Player::delete_game() {
    joined_game = nullptr;
}


Player::~Player() {
    if (joined_game != nullptr) {
        joined_game->remove_player(this);
    }
}


bool
Player::has_joined_game() const {
    return joined_game;
}

ushort
Player::get_id() const {
    return id;
}

ushort
Player::get_position() const {
    return position;
}


Observer*
Player::observer() const {
    return nullptr;
}


bool
Player::check_rules(const Rules& rules) const {
    return true;
}


void
Player::error(string message) {
    std::cout << "[PLAYER#" << id << ":ERROR]" << message << endl;
}


// Reading

string
Player::player_type() const {
    return "";
}

ostream&
operator<<(ostream& os, const Player& player) {
    return os << "<Player#" << player.id << ":" << player.player_type() << ">";
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