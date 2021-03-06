#ifndef RANDOM_PLAYER_HPP
#define RANDOM_PLAYER_HPP

#include "game/action.hpp"
#include "game/player.hpp"
#include "global.hpp"
#include "state/state.hpp"
#include "utils/random.hpp"

class RandomPlayer : public Player {
private:
    rng randomness;
    ushort_range range;

public:
    const bool smart = true;

    RandomPlayer(bool smart = true);
    RandomPlayer(int seed, bool smart = true);

    virtual std::shared_ptr<Player> copy() const override;

    virtual Action play(const State& state) override;

    virtual std::string player_type() const override;
};

#endif //RANDOM_PLAYER_HPP
