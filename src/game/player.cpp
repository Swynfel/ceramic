#include "player.hpp"

#include "game.hpp"
#include <sstream>

void
Player::join_game(ushort _id) {
    id = _id;
}

ushort
Player::get_id() const {
    return id;
}


Action
Player::play(const State& state) {
    auto rules = state.get_rules();
    for (int place = rules->tile_types; place >= 0; place--) {
        for (int pick = 0; pick <= rules->factory_count(); pick++) {
            for (int color = 0; color < rules->tile_types; color++) {
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
    return os << '<Player#' << player.id << '>';
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