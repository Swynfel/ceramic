#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "action.hpp"
#include "global.hpp"
#include "state/state.hpp"

class Player {
protected:
    ushort id;

public:
    void join_game(ushort id);

    ushort get_id() const;

    Action play(const State& state);
    void error(string message);

    // Reading
    friend ostream& operator<<(ostream& os, const Player& player);
    string str() const;
    string repr() const;
};

#endif //PLAYER_HPP
