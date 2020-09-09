#include "game.hpp"

uniform_int_distribution<> int_dist = uniform_int_distribution<>(1, 6);

// Constuctors

Game::Game(const Rules& rules)
  : state(rules)
  , randomness()
  , range() {}

Game::Game()
  : Game(Rules::DEFAULT) {}


// Private methods

int
Game::rand(int min, int max) {
    return int_dist(randomness, uniform_int_distribution<>::param_type(0, max - 1));
}

// Should only be called if there is at least
// one tile in bag
Tile
Game::pull_one_random_tile() {
    int v = rand(0, state.bag.total());
    int i;
    for (i = 0; i < state.rules.tile_types - 1; i++) {
        v -= state.bag[i];
        if (v < 0) {
            break;
        }
    }
    Tile tile = Tile(i);
    state.bag -= tile;
    return tile;
}

Tiles
Game::pull_random_tiles(int count) {
    if (count == 0) {
        return Tiles::ZERO;
    }
    Tiles result;
    // If bag almost empty
    int available = state.bag.total();
    if (available <= count) {
        // Pull out the rest
        if (available > 0) {
            result = state.bag;
            count -= available;
        }
        // If done already
        if (count == 0) {
            state.bag = Tiles::ZERO;
            // Return
            return result;
        }
        // Else empty bin into bag and try again
        state.bag = state.bin;
        state.bin = Tiles::ZERO;
        available = state.bag.total();
        // If bag still almost empty
        if (available <= count) {
            // Pull out the rest
            if (available > 0) {
                result = state.bag;
                state.bag = Tiles::ZERO;
            }
            // And return
            return result;
        }
    }
    // Pull out tile one by one
    for (int i = 0; i < count; i++) {
        result += pull_one_random_tile();
    }
    return result;
}


// Public methods

void
Game::start() {
    state.reset();
    int first_player = rand(0, state.rules.player_count - 1);
    state.get_panel_mut(first_player).set_first_token(true);
    start_round();
}

void
Game::start_round() {
    for (auto& factory : state.factories) {
        factory.set_tiles(pull_random_tiles(state.rules.factory_tiles));
        if (state.bag.is_empty() && state.bin.is_empty()) {
            break;
        }
    }
}

void
Game::end_round() {
}

bool
Game::legal(const Action& action, const State& state) {
    const Rules& rules = state.get_rules();
    // Check action is coherent with current rules
    if (action.color >= rules.tile_types || action.pick > rules.factories()) {
        return false;
    }
    // If action is not "throwing away"
    if (action.place > 0) {
        Panel panel = state.get_panel(state.get_current_player());
        Tile color = panel.get_pyramid().color(action.place);
        // Check action color matches tile color present on the line
        if (color != Tile::NONE && color != action.color) {
            return false;
        }
    }
    Tiles picked;
    if (action.pick == 0) {
        picked = state.get_center();
    } else {
        picked = state.get_factory(action.pick);
    }
    // Check action color is present in picked Center / Factory
    if (!picked.has_color(action.color)) {
        return false;
    }
    return true;
}

void
Game::apply(const Action& action, State& state) {
    if (action.color >= state.get_rules().tile_types) {
        throw std::invalid_argument("No tile of color " + action.color.str() + " in game");
    }
    Panel panel = state.get_panel_mut(state.get_current_player());
    Tiles picked;
    if (action.pick == 0) {
        picked = state.get_center_mut();
    } else {
        picked = state.get_factory_mut(action.pick);
    }
    // Check action color is present in picked Center / Factory
    if (picked.is_empty()) {
        throw std::invalid_argument("Factory " + picked.str() + " is empty");
    }
    if (!picked.has_color(action.color)) {
        throw std::invalid_argument("Factory " + picked.str() + " has no tiles of color " + to_string(action.pick));
    }
    int count = picked[action.color];
    Pyramid pyramid = panel.get_pyramid_mut();
    int overflow_count = max(0, count - pyramid.amount_remaining(action.place));
    pyramid.set_line(action.place, count - overflow_count, action.color); // Can raise invalid_argument

    // Action is confirmed to be legal
    // Everything that follows should not raise exceptions
    if (overflow_count > 0) {
        state.get_bin_mut() += Tiles(action.color, overflow_count);
    }
    Center center = state.get_center_mut();
    picked[action.color] = 0;
    if (action.pick == 0) {
        if (center.first_token) {
            center.first_token = false;
            overflow_count++;
            panel.set_first_token(true);
        }
    } else {
        Factory factory = state.get_factory_mut(action.pick);
        center += factory;
        factory.set_tiles(Tiles::ZERO);
    }

    state.next_player();
}