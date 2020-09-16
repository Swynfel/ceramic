#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "action.hpp"
#include "global.hpp"
#include "observer.hpp"
#include "state/state.hpp"

class Game;
class Player {
private:
    friend class Game;

public:
    virtual bool check_rules(const Rules& rules) const;

    virtual std::shared_ptr<Observer> observer();

    virtual Action play(const State& state) = 0;
    virtual void error(string message);

    // Reading
    virtual string player_type() const;
    friend ostream& operator<<(ostream& os, const Player& player);
    string str() const;
    string repr() const;
};

#endif //PLAYER_HPP
