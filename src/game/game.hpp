#ifndef GAME_HPP
#define GAME_HPP

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include "action.hpp"
#include "observer.hpp"
#include "player.hpp"
#include "rules/rules.hpp"
#include "state/state.hpp"
#include "utils/random.hpp"

class Game {
private:
    State state;
    vector<Player*> players;
    vector<Observer*> observers;
    vector<ushort> order;
    rng randomness;
    ushort_range range;

    Tile pull_one_random_tile();
    Tiles pull_random_tiles(int count);

public:
    Game();
    Game(const std::shared_ptr<Rules>& rules);
    Game(const std::shared_ptr<Rules>& rules, int seed);
    Game(const std::shared_ptr<Rules>& rules, vector<Player*> players);
    ~Game();

    const State get_state() const;

    ushort players_missing() const;
    bool has_enough_players() const;
    void add_player(Player* player);
    void add_players(vector<Player*> players);
    void remove_player(Player* player);
    void add_observer(Observer* observer);
    void remove_observer(Observer* observer);

    void reset();
    void start_round();
    void end_round();
    void score_final();

    void next_player();
    void roll_round();
    void roll_game();

    void setup_factories();
    void score_panels();
    void static score_panels(State& state);
    void apply_first_token();
    void static apply_first_token(State& state);

    bool legal(Action action) const;
    void apply(Action action);

    bool static legal(Action action, const State& state);
    void static apply(Action action, State& state);
    vector<Action> static all_legal(const State& state);
};

#endif //GAME_HPP
