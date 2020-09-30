#ifndef MONTE_CARLO_PLAYER
#define MONTE_CARLO_PLAYER

#include "game/action.hpp"
#include "game/player.hpp"
#include "global.hpp"
#include "random_player.hpp"
#include "round_heuristic.hpp"
#include "state/state.hpp"
#include "utils/random.hpp"
#include <cmath>

class MonteCarloPlayer : public Player {
private:
    std::shared_ptr<Player> sampling_player;
    rng randomness = rng(random_seed());

    float state_score(Game& game, const State& state, int player);
    int select_ucb(int n, const std::vector<float>& score_sums, const std::vector<int>& count) const;

public:
    constexpr static int DEFAULT_ROLLOUTS = 1000;
    constexpr static float DEFAULT_C = M_SQRT2;
    RoundHeuristic heuristic{};
    int rollouts;
    bool until_round;
    bool smart = true;
    float c = DEFAULT_C;

    MonteCarloPlayer(bool until_round = true, int rollouts = DEFAULT_ROLLOUTS);
    MonteCarloPlayer(std::shared_ptr<Player> player, bool until_round = true, int rollouts = DEFAULT_ROLLOUTS);
    MonteCarloPlayer(const MonteCarloPlayer& other);

    virtual std::shared_ptr<Player> copy() override;

    virtual Action play(const State& state) override;

    virtual std::string player_type() const override;
};

#endif //MONTE_CARLO_PLAYER
