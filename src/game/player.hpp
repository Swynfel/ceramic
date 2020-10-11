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
    virtual ~Player() = default;

    virtual bool check_rules(const Rules& rules) const;

    virtual std::shared_ptr<Observer> observer();
    virtual std::shared_ptr<Player> copy() const = 0;

    virtual Action play(const State& state) = 0;
    virtual void error(std::string message);

    // Reading
    virtual std::string player_type() const;
    friend std::ostream& operator<<(std::ostream& os, const Player& player);
    std::string str() const;
    std::string repr() const;
};

#endif //PLAYER_HPP
