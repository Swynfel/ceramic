#include "state.hpp"

#include <sstream>

State::State(std::shared_ptr<const Rules> rules)
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


State&
State::operator=(const State& other) {
    if (rules != other.rules) {
        throw std::logic_error("Cannot assign state with different rules");
    }
    for (int f = 0; f < rules->factory_count(); f++) {
        factories[f] = other.factories[f];
    }
    for (int p = 0; p < rules->player_count; p++) {
        panels[p] = other.panels[p];
    }
    return *this;
}

bool
operator==(const State& left, const State& right) {
    return left.rules == right.rules &&
           left.factories == right.factories &&
           left.panels == right.panels &&
           left.center == right.center &&
           left.player == right.player &&
           left.bag == right.bag &&
           left.bin == right.bin;
}

bool
operator!=(const State& left, const State& right) {
    return !(left == right);
}


void
State::assert_player_id(ushort id) const {
    if (id >= rules->player_count) {
        throw std::invalid_argument("No player with id '" + std::to_string(id) + "', as there is only '" + std::to_string(rules->player_count) + "' players.");
    }
}

void
State::assert_factory_id(ushort id) const {
    if (id == 0) {
        throw std::invalid_argument("No factory with id '0', as it designates the Center.");
    }
    if (id > factories.size()) {
        throw std::invalid_argument("No factory with id '" + std::to_string(id) + "', as there is only '" + std::to_string(factories.size()) + "' factories.");
    }
}

void
State::reset() {
    bag = Tiles(std::vector<ushort>(rules->tile_types, rules->tile_count));
    for (Factory& factory : factories) {
        factory.tiles = Tiles::ZERO;
    }
    for (Panel& panel : panels) {
        panel.clear();
    }
}

const std::shared_ptr<const Rules>&
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
    result += center.tiles;
    for (const Factory& factory : factories) {
        result += factory.tiles;
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
State::is_running_out_of_tiles() const {
    Tiles tiles = bag + bin;
    for (ushort color = 0; color < rules->tile_types; color++) {
        if (tiles[color] == 0) {
            return true;
        }
    }
    return false;
}

bool
State::is_round_finished() const {
    if (!center.tiles.is_empty()) {
        return false;
    }
    for (const Factory& factory : factories) {
        if (!factory.tiles.is_empty()) {
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
    return is_running_out_of_tiles();
}

std::vector<ushort>
State::highest_score_players() const {
    ushort highest_score = 0;
    std::vector<ushort> highest_players;
    ushort seat = 0;
    for (const Panel& panel : panels) {
        ushort score = panel.get_score();
        if (score >= highest_score) {
            if (score != highest_score) {
                highest_players.clear();
            }
            highest_players.push_back(seat);
            highest_score = score;
        }
        seat++;
    }
    return highest_players;
}

ushort
State::winning_player() const {
    // The winner is the player with the highest score
    std::vector<ushort> highest_players = highest_score_players();
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
    return winner;
}

// Reading

std::ostream&
operator<<(std::ostream& os, const State& state) {
    os << "bag:" << state.bag << "\nbin:" << state.bin << "\n";
    os << "--------------------\n";
    os << state.center << '\n';
    for (const Factory& factory : state.factories) {
        os << factory << '\n';
    }
    os << "--------------------\n";
    bool finished = state.is_game_finished();
    ushort player = finished ? state.winning_player() : state.player;
    ushort id = 0;
    for (const Panel& panel : state.panels) {
        if (player == id) {
            os << (finished ? "(winner)" : "(current)") << '\n';
        }
        os << panel << '\n';
        id++;
    }
    return os;
}

std::string
State::str() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

std::string
State::repr() const {
    std::ostringstream os;
    os << "<Ceramic State " << this << ">";
    return os.str();
}