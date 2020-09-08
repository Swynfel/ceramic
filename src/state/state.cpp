#include "state.hpp"

State::State(const Rules& rules)
  : rules(rules)
  , factories(rules.factories(), Factory(rules.player_count))
  , panels(rules.player_count, Panel(rules))
  , bag()
  , bin() {}

void
State::start() {
    bag = Tiles(vector<ushort>(rules.tile_types, rules.tile_count));
    for (Factory& factory : factories) {
        factory.tiles = Tiles::ZERO;
    }
    for (Panel& panel : panels) {
        panel.clear();
    }
}

void
State::start_round() {
}

const Rules&
State::get_rules() const {
    return rules;
}

const ushort
State::get_current_player() const {
    return player;
}

void
State::set_current_player(ushort id) {
    if (id >= rules.player_count) {
        throw std::invalid_argument("No player with id '" + to_string(id) + "', as there is only '" + to_string(rules.player_count) + "' players.");
    }
    player = id;
}