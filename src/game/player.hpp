#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "action.hpp"
#include "global.hpp"
#include "state/state.hpp"

class Player {
protected:
    bool joined_game = false;
    ushort id;
    ushort position;

public:
    bool check_rules(const Rules& rules);
    bool join_game(const Rules& rules, ushort id);
    void start_game(ushort position);
    void new_round(const State& state);
    void action_played(Action action);
    void end_game(const State& state, ushort winner_id, ushort winner_position);

    ushort get_id() const;
    ushort get_position() const;
    bool has_joined_game() const;

    Action play(const State& state);
    void error(string message);

    // Reading
    friend ostream& operator<<(ostream& os, const Player& player);
    string str() const;
    string repr() const;
};

#endif //PLAYER_HPP
