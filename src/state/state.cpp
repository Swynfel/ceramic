#include "state.hpp"

#include <sstream>

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
State::assert_player_id(const ushort id) const {
    if (id >= rules.player_count) {
        throw std::invalid_argument("No player with id '" + to_string(id) + "', as there is only '" + to_string(rules.player_count) + "' players.");
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

const Factory&
State::get_factory(const ushort id) const {
    return factories[id];
}

Factory&
State::get_factory_mut(const ushort id) {
    return factories[id];
}

const Panel&
State::get_panel(const ushort id) const {
    assert_player_id(id);
    return panels[id];
}

Panel&
State::get_panel_mut(const ushort id) {
    assert_player_id(id);
    return panels[id];
}

void
State::set_current_player(ushort id) {
    assert_player_id(id);
    player = id;
}

void
State::next_player() {
    if (player + 1 >= rules.player_count) {
        player = 0;
    } else {
        player++;
    }
}

ostream&
operator<<(ostream& os, const State& state) {
    return os << "<Ceramic State " << &state << ">";
}

string
State::str() const {
    ostringstream os;
    os << *this;
    return os.str();
}

string
State::repr() const {
    return str();
}