#include "game.hpp"

// Constuctors

Game::Game()
  : Game(Rules::DEFAULT) {}

Game::Game(std::shared_ptr<Rules> rules)
  : Game(rules, random_seed()) {}

Game::Game(std::shared_ptr<Rules> rules, int seed)
  : state(rules)
  , players()
  , observers()
  , order()
  , randomness(seed)
  , range() {
    reset();
}

Game::Game(std::shared_ptr<Rules> rules, vector<std::shared_ptr<Player>> players)
  : state(rules)
  , players()
  , observers()
  , order()
  , randomness(random_seed())
  , range() {
    if (players.size() > rules->player_count) {
        throw invalid_argument("Too many players for rules");
    }
    for (std::shared_ptr<Player> player : players) {
        players.push_back(std::move(player));
        std::shared_ptr<Observer> observer = player->observer();
        if (observer != nullptr) {
            observers.push_back(std::move(observer));
        }
    }
    reset();
}


// Private methods

const State&
Game::get_state() const {
    return state;
}

// Should only be called if there is at least
// one tile in bag
Tile
Game::pull_one_random_tile() {
    int v = random_range(randomness, range, 0, state.bag.total());
    int i;
    for (i = 0; i < state.rules->tile_types - 1; i++) {
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


ushort
Game::players_missing() const {
    return state.rules->player_count - players.size();
}
bool
Game::has_enough_players() const {
    return players_missing() == 0;
}

void
Game::add_player(std::shared_ptr<Player> player) {
    if (has_enough_players()) {
        throw logic_error("Already enough players");
    }
    if (player->check_rules(*state.rules)) {
        players.push_back(player);
        std::shared_ptr<Observer> observer = player->observer();
        if (observer != nullptr) {
            observers.push_back(std::move(observer));
        }
    } else {
        throw runtime_error("Failed adding player");
    }
}

void
Game::add_players(vector<std::shared_ptr<Player>> _players) {
    if (players_missing() < _players.size()) {
        throw out_of_range("Not enough seats for this many players");
    }
    for (const std::shared_ptr<Player>& player : _players) {
        add_player(player);
    }
}

void
Game::remove_player(std::shared_ptr<Player> player) {
    vector<std::shared_ptr<Player>>::iterator p = std::find(players.begin(), players.end(), player);
    if (p != players.end()) {
        players.erase(p);
    }
}

void
Game::add_observer(std::shared_ptr<Observer> observer) {
    observers.push_back(observer);
}

void
Game::remove_observer(std::shared_ptr<Observer> observer) {
    vector<std::shared_ptr<Observer>>::iterator obs = std::find(observers.begin(), observers.end(), observer);
    if (obs != observers.end()) {
        observers.erase(obs);
    }
}

void
Game::reset() {
    state.reset();
    if (order.empty()) {
        for (ushort i = 0; i < state.rules->player_count; i++) {
            order.push_back(i);
        }
    }
    std::shuffle(order.begin(), order.end(), randomness);
    state.set_current_player(0);
}

void
Game::start_round() {
    setup_factories();
}

void
Game::end_round() {
    score_panels();
    apply_first_token();
}

void
Game::score_final() {
    for (Panel& panel : state.panels) {
        const Wall& wall = panel.get_wall();
        panel.add_score(wall.final_score_bonus());
    }
}


void
Game::next_player() {
    state.next_player();
}

void
Game::roll_round() {
    if (!has_enough_players()) {
        throw logic_error("Not enough players to play a round");
    }
    while (!state.is_round_finished()) {
        const std::shared_ptr<Player>& player = players[order[state.player]];
        Action action = player->play(state);
        try {
            apply(action);
            for (const std::shared_ptr<Observer>& observer : observers) {
                observer->action_played(action);
            }
            state.next_player();
        } catch (exception e) {
            player->error(e.what());
            cout << e.what() << endl;
        }
    }
}

void
Game::roll_game() {
    if (!has_enough_players()) {
        throw logic_error("Not enough players to start the game");
    }
    reset();
    for (const std::shared_ptr<Observer>& observer : observers) {
        observer->start_game(order);
    }
    while (!state.is_game_finished()) {
        start_round();
        for (const std::shared_ptr<Observer>& observer : observers) {
            observer->new_round(state);
        }
        roll_round();
        end_round();
    }
    score_final();
    ushort winner_position = state.winning_player();
    for (const std::shared_ptr<Observer>& observer : observers) {
        observer->end_game(state, winner_position);
    }
}


void
Game::setup_factories() {
    // Fill factories
    for (auto& factory : state.factories) {
        factory.set_tiles(pull_random_tiles(state.rules->factory_tiles));
        if (state.bag.is_empty() && state.bin.is_empty()) {
            break;
        }
    }
    state.center.set_tiles(Tiles::ZERO);
    state.center.first_token = true;
}

void
Game::score_panels() {
    score_panels(state);
}

void
Game::score_panels(State& state) {
    for (Panel& panel : state.panels) {
        int score = 0;
        Pyramid& pyramid = panel.get_pyramid_mut();
        Wall& wall = panel.get_wall_mut();
        for (int line = 1; line <= state.rules->tile_types; line++) {
            if (pyramid.is_filled(line)) {
                // Take tiles of pyramid line
                Tile color = pyramid.color(line);
                Tiles tiles = pyramid.get_line(line);
                pyramid.clear_line(line);
                // Place then on wall and add score
                if (!wall.line_has_color(line, color)) {
                    // The wall should never have the color on its line
                    // But we check in case it was manually edited, out of Game
                    score += wall.place_line_color(line, color);
                    tiles -= color;
                }
                state.bin += tiles;
            }
        }
        score -= panel.get_penalty();
        panel.clear_floor();
        panel.add_score(score);
    }
}

void
Game::apply_first_token() {
    apply_first_token(state);
}

void
Game::apply_first_token(State& state) {
    ushort id = 0;
    for (Panel& panel : state.panels) {
        if (panel.get_first_token()) {
            panel.set_first_token(false);
            state.set_current_player(id);
            return;
        }
        id++;
    }
}


bool
Game::legal(Action action) const {
    return legal(action, state);
}

void
Game::apply(Action action) {
    apply(action, state);
}


bool
Game::legal(Action action, const State& state) {
    const std::shared_ptr<Rules>& rules = state.get_rules();
    // Check action is coherent with current rules
    if (ushort(action.color) >= rules->tile_types || action.pick > rules->factory_count()) {
        return false;
    }
    // If action is not "throwing away", check color can be placed on line
    if (action.place > 0 && !state.get_panel(state.player).legal_line(action.place, action.color)) {
        return false;
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


vector<Action>
Game::all_legal(const State& state) {
    vector<Action> legal_actions{};
    std::shared_ptr<Rules> rules = state.get_rules();
    const Panel& panel = state.get_panel(state.player);
    for (ushort pick = 0; pick <= rules->factory_count(); pick++) {
        Tiles tiles = (pick == 0) ? (Tiles)state.center : (Tiles)state.get_factory(pick);
        if (tiles.is_empty()) {
            continue;
        }
        for (ushort color = 0; color < rules->tile_types; color++) {
            Tile tile = Tile(color);
            if (!tiles.has_color(tile)) {
                continue;
            }
            for (ushort place = 0; place <= rules->tile_types; place++) {
                if (place > 0 && !panel.legal_line(place, tile)) {
                    continue;
                }
                legal_actions.push_back(Action{ .pick = pick, .color = tile, .place = place });
            }
        }
    }
    return legal_actions;
}

void
Game::apply(Action action, State& state) {
    // Check action is coherent with current rules
    if (ushort(action.color) >= state.rules->tile_types) {
        throw std::invalid_argument("No tile of color " + action.color.str() + " in game");
    }

    Panel& panel = state.get_panel_mut(state.get_current_player());
    Tiles& picked = (action.pick == 0) ? (Tiles&)state.get_center_mut() : (Tiles&)state.get_factory_mut(action.pick);

    // Check action color is present in picked Center / Factory
    if (picked.is_empty()) {
        throw std::invalid_argument(picked.repr() + " is empty");
    }
    if (!picked.has_color(action.color)) {
        throw std::invalid_argument(picked.repr() + " has no tiles of color " + to_string(action.pick));
    }
    // If action is not "throwing away", check color can be placed on line
    if (action.place > 0 && !panel.legal_line(action.place, action.color)) {
        throw std::invalid_argument("Cannot place tile " + action.color.str() + " on line " + to_string(action.place));
    }

    // Action is confirmed to be legal
    // Everything that follows should not raise exceptions
    Center& center = state.get_center_mut();

    // Remove all tiles of corresponding color from picked
    int count = picked[action.color];
    picked[action.color] = 0;


    int overflow_count;
    if (action.place == 0) {
        // If directly thrown away
        overflow_count = count;
    } else {
        // Else pyramid, some will be placed in a line
        Pyramid& pyramid = panel.get_pyramid_mut();
        overflow_count = max(0, count - pyramid.amount_remaining(action.place));
        // Determine new amount of tiles on pyramid line
        count -= overflow_count;
        count += pyramid.amount(action.place);
        // And set it
        pyramid.set_line(action.place, count, action.color);
    }
    if (overflow_count > 0) {
        // Throw excess tiles in bin
        state.get_bin_mut() += Tiles(action.color, overflow_count);
    }

    if (action.pick == 0) {
        // If center, check if first token is taken
        if (center.first_token) {
            center.first_token = false;
            overflow_count++;
            panel.set_first_token(true);
        }
    } else {
        // If factory, move all tiles to center
        Factory& factory = state.get_factory_mut(action.pick);
        center += factory;
        factory.set_tiles(Tiles::ZERO);
    }
    // Add thrown tiles as floor penalty
    panel.add_floor(overflow_count);
}