#ifndef ANALYSIS_PLAYER_HPP
#define ANALYSIS_PLAYER_HPP

#include "game/action.hpp"
#include "game/player.hpp"
#include "global.hpp"
#include "state/state.hpp"
#include <atomic>

class AnalysisPlayer : public Player {
public:
    AnalysisPlayer(std::shared_ptr<Player> player, bool analysis = true);

    const std::shared_ptr<Player> analysed_player;
    bool analysis = true;

    std::atomic<int> move_counter{ 0 };
    std::atomic<long long> time{ 0 };

    virtual std::shared_ptr<Player> copy() const override;

    Action play(const State& state) override;

    void error(std::string error) override;

    std::string player_type() const override;

    std::shared_ptr<Observer> observer() override;
};

#endif //ANALYSIS_PLAYER_HPP
