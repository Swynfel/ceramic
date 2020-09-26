#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include "action.hpp"
#include "global.hpp"
#include "state/state.hpp"

class Game;

class Observer {
public:
    void virtual start_game(std::vector<ushort> order) {}
    void virtual new_round(const State& state) {}
    void virtual action_played(Action action) {}
    void virtual end_game(const State& state, ushort winner_position) {}
};

#endif //OBSERVER_HPP
