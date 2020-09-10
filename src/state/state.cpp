#include "state.hpp"

#include <sstream>

State::State(const std::shared_ptr<Rules>& rules)
  : rules(rules)
  , center()
  , factories()
  , panels(rules->player_count, Panel(rules))
  , bag()
  , bin() {
    int factory_count = rules->factory_count();
    for (int i = 1; i < factory_count; i++) {
        factories.push_back(Factory(i));
    }
}

State::State(const State& state)
  : rules(state.rules)
  , center(state.center)
  , factories()
  , panels()
  , bag(state.bag)
  , bin(state.bin) {
    for (auto& factory : state.factories) {
        factories.push_back(Factory(factory));
    }
    for (auto& panel : state.panels) {
        panels.push_back(Panel(panel));
    }
}


void
State::assert_player_id(ushort id) const {
    if (id >= rules->player_count) {
        throw std::invalid_argument("No player with id '" + to_string(id) + "', as there is only '" + to_string(rules->player_count) + "' players.");
    }
}

void
State::assert_factory_id(ushort id) const {
    if (id == 0) {
        throw std::invalid_argument("No factory with id '0', as it designates the Center.");
    }
    if (id > factories.size()) {
        throw std::invalid_argument("No factory with id '" + to_string(id) + "', as there is only '" + to_string(factories.size()) + "' factories.");
    }
}

void
State::reset() {
    bag = Tiles(vector<ushort>(rules->tile_types, rules->tile_count));
    for (Factory& factory : factories) {
        factory.set_tiles(Tiles::ZERO);
    }
    for (Panel& panel : panels) {
        panel.clear();
    }
}

const std::shared_ptr<Rules>&
State::get_rules() const {
    return rules;
}

ushort
State::get_current_player() const {
    return player;
}


const Center&
State::get_center() const {
    return center;
}

Center&
State::get_center_mut() {
    return center;
}

const Factory&
State::get_factory(ushort id) const {
    assert_factory_id(id);
    return factories[id - 1];
}

Factory&
State::get_factory_mut(ushort id) {
    assert_factory_id(id);
    return factories[id - 1];
}

const Panel&
State::get_panel(ushort id) const {
    assert_player_id(id);
    return panels[id];
}

Panel&
State::get_panel_mut(ushort id) {
    assert_player_id(id);
    return panels[id];
}


Tiles
State::get_bag() const {
    return bag;
}

Tiles&
State::get_bag_mut() {
    return bag;
}

Tiles
State::get_bin() const {
    return bin;
}

Tiles&
State::get_bin_mut() {
    return bin;
}


void
State::set_current_player(ushort id) {
    assert_player_id(id);
    player = id;
}

void
State::next_player() {
    if (player + 1 >= rules->player_count) {
        player = 0;
    } else {
        player++;
    }
}


// Reading

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