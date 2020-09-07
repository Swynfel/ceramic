#ifndef GAME_HPP
#define GAME_HPP

#include <boost/random/mersenne_twister.hpp>
#include <memory>
#include <vector>

#include "../state/rules.hpp"
#include "../state/state.hpp"
#include "action.hpp"

typedef boost::random::mt11213b rng;

class Game {
private:
    rng randomness;
    State state;

public:
    Game();
    Game(const std::shared_ptr<Rules>& rules);
    Game(const Rules& rules);

    void start();
    void start_round();

    bool static legal(const Action& action, const State& state);
    void static apply(const Action& action, State& state);
    void static unsafe_apply(const Action& action, State& state);

    friend class Play;
};

#endif //GAME_HPP
