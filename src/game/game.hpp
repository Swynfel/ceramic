#ifndef GAME_HPP
#define GAME_HPP

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_smallint.hpp>
#include <limits>
#include <memory>
#include <vector>

#include "../rules/rules.hpp"
#include "../state/state.hpp"
#include "action.hpp"

using namespace boost::random;

typedef mt11213b rng;

class Game {

private:
    State state;
    rng randomness;
    uniform_int_distribution<> range;

    int rand(int min = 0, int max = std::numeric_limits<int>::max());
    Tile pull_one_random_tile();
    Tiles pull_random_tiles(int count);

public:
    Game();
    Game(const std::shared_ptr<Rules>& rules);

    void start();
    void start_round();
    void end_round();

    bool static legal(const Action& action, const State& state);
    void static apply(const Action& action, State& state);
};

#endif //GAME_HPP
