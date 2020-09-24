#ifndef MONTE_CARLO_PLAYER
#define MONTE_CARLO_PLAYER

#include "game/action.hpp"
#include "game/player.hpp"
#include "global.hpp"
#include "random_player.hpp"
#include "state/state.hpp"
#include "utils/random.hpp"

class MonteCarloPlayer : public Player {
private:
    std::shared_ptr<Player> sampling_player;

    float round_score(Game& game, const State& state, int player);
    float game_score(Game& game, const State& state, int player);

public:
    const static int DEFAULT_ROLLOUTS = 100;
    int rollouts;
    bool until_round;

    MonteCarloPlayer(bool until_round = true, int rollouts = DEFAULT_ROLLOUTS);
    MonteCarloPlayer(std::shared_ptr<Player> player, bool until_round = true, int rollouts = DEFAULT_ROLLOUTS);

    Action play(const State& state) override;

    virtual string player_type() const override;
};

#endif //MONTE_CARLO_PLAYER
