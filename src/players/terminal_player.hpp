#ifndef TERMINAL_PLAYER
#define TERMINAL_PLAYER

#include "game/action.hpp"
#include "game/player.hpp"
#include "global.hpp"
#include "state/state.hpp"

class TerminalPlayer : public Player {
private:
    void help() const;

public:
    Action play(const State& state) override;

    void error(string error) override;

    virtual string player_type() const override;
};

#endif //TERMINAL_PLAYER
