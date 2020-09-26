#ifndef FIRST_LEGAL_PLAYER_HPP
#define FIRST_LEGAL_PLAYER_HPP

#include "game/action.hpp"
#include "game/player.hpp"
#include "global.hpp"
#include "state/state.hpp"

class FirstLegalPlayer : public Player {
public:
    Action play(const State& state) override;

    virtual std::string player_type() const override;
};

#endif //FIRST_LEGAL_PLAYER_HPP
