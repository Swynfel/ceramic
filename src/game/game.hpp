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

class Arena;
class Game {
private:
    friend class Arena;

    State state;
    vector<std::shared_ptr<Player>> players;
    vector<std::shared_ptr<Observer>> observers;
    vector<ushort> order;
    rng randomness;
    ushort_range range;

    Tile pull_one_random_tile();
    Tiles pull_random_tiles(int count);

    vector<Action> static all_legal_between(const State& state, ushort begin_place, ushort end_place);

public:
    Game();
    Game(std::shared_ptr<const Rules> rules);
    Game(std::shared_ptr<const Rules> rules, int seed);
    Game(std::shared_ptr<const Rules> rules, vector<std::shared_ptr<Player>> players);

    const State& get_state() const;

    ushort players_missing() const;
    bool has_enough_players() const;
    void add_player(std::shared_ptr<Player> player);
    void add_players(vector<std::shared_ptr<Player>> players);
    void remove_player(std::shared_ptr<Player> player);
    void add_observer(std::shared_ptr<Observer> observer);
    void remove_observer(std::shared_ptr<Observer> observer);

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
    vector<Action> static all_smart_legal(const State& state);
    vector<Action> static all_penalty_legal(const State& state);
};

#endif //GAME_HPP
