#include "player.hpp"

#include "game.hpp"
#include <sstream>

bool
Player::check_rules(const Rules& rules) {
    return true;
}

void
Player::start_game(ushort _position) {
    position = _position;
}

bool
Player::join_game(const Rules& rules, ushort _id) {
    if (joined_game) {
        throw runtime_error("Already joined game");
    }
    if (!check_rules(rules)) {
        return false;
    }
    joined_game = true;
    id = _id;
    return true;
}

void
Player::new_round(const State& state) {}

void
Player::action_played(Action action) {}

void
Player::end_game(const State& state, ushort winner_id, ushort winner_position) {}


ushort
Player::get_id() const {
    return id;
}

ushort
Player::get_position() const {
    return position;
}

bool
Player::has_joined_game() const {
    return joined_game;
}


Action
Player::play(const State& state) {
    auto rules = state.get_rules();
    for (ushort place = rules->tile_types; place >= 0; place--) {
        for (ushort pick = 0; pick <= rules->factory_count(); pick++) {
            for (ushort color = 0; color < rules->tile_types; color++) {
                Action action = Action{ .pick = pick, .color = color, .place = place };
                if (Game::legal(action, state)) {
                    return action;
                }
            }
        }
    }
    throw runtime_error("No action was legal");
}

void
Player::error(string message) {
    cout << "[PLAYER#" << id << ":ERROR]" << message << endl;
}

ostream&
operator<<(ostream& os, const Player& player) {
    return os << "<Player#" << player.id << ">";
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