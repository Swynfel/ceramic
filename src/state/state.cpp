#include "state.hpp"

#include <sstream>

State::State(std::shared_ptr<Rules> rules)
  : rules(rules)
  , center()
  , factories()
  , panels(rules->player_count, Panel(std::move(rules)))
  , bag()
  , bin()
  , player() {
    int factory_count = this->rules->factory_count();
    for (int i = 1; i <= factory_count; i++) {
        factories.push_back(Factory(i));
    }
}

State::State(const State& state)
  : rules(state.rules)
  , center(state.center)
  , factories()
  , panels()
  , bag(state.bag)
  , bin(state.bin)
  , player(state.player) {
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


Tiles
State::get_total_tiles() const {
    Tiles result = Tiles();
    for (const Panel& panel : panels) {
        const Pyramid& pyramid = panel.get_pyramid();
        for (int line = 1; line <= rules->tile_types; line++) {
            result += pyramid.get_line(line);
        }
        auto wall_tiles = panel.get_wall().get_tiles();
        for (Tile tile : wall_tiles) {
            result += tile;
        }
    }
    result += center;
    for (const Factory& factory : factories) {
        result += factory;
    }

    result += bag;
    result += bin;
    return result;
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


bool
State::is_round_finished() const {
    if (!center.is_empty()) {
        return false;
    }
    for (const Factory& factory : factories) {
        if (!factory.is_empty()) {
            return false;
        }
    }
    return true;
}


bool
State::is_game_finished() const {
    for (const Panel& panel : panels) {
        if (panel.get_wall().completed_line_count() > 0) {
            return true;
        }
    }
    return false;
}

vector<ushort>
State::highest_score_players() const {
    ushort highest_score = 0;
    vector<ushort> highest_players;
    ushort seat = 0;
    for (const Panel& panel : panels) {
        if (panel.get_score() >= highest_score) {
            if (panel.get_score() != highest_score) {
                highest_players.clear();
            }
            highest_players.push_back(seat);
        }
        seat++;
    }
    return highest_players;
}

#include <iostream>

ushort
State::winning_player() const {
    // The winner is the player with the highest score
    vector<ushort> highest_players = highest_score_players();
    if (highest_players.size() == 1) {
        return highest_players.front();
    }
    // Break ties by number of completed lines on wall
    ushort winner;
    ushort highest_count = 0;
    for (ushort player : highest_players) {
        const Panel& panel = panels[player];
        ushort count = panel.get_wall().completed_line_count();
        // Break ties again by taking last player with the highest_count
        if (count >= highest_count) {
            highest_count = count;
            winner = player;
        }
    }
    return player;
}

// Reading

ostream&
operator<<(ostream& os, const State& state) {
    os << state.center << endl;
    for (const Factory& factory : state.factories) {
        os << factory << endl;
    }
    os << "--------------------" << endl;
    for (const Panel& panel : state.panels) {
        os << panel << endl;
    }
    return os;
}

string
State::str() const {
    ostringstream os;
    os << *this;
    return os.str();
}

string
State::repr() const {
    ostringstream os;
    os << "<Ceramic State " << this << ">";
    return os.str();
}