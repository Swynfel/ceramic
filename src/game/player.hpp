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
    bool join_game(Game* game, ushort id);
    void set_position(ushort position);
    void delete_game();

protected:
    Game* joined_game = nullptr;
    ushort id;
    ushort position;

public:
    ~Player();

    bool has_joined_game() const;
    ushort get_id() const;
    ushort get_position() const;

    virtual bool check_rules(const Rules& rules) const;

    virtual Observer* observer() const;

    virtual Action play(const State& state) = 0;
    virtual void error(string message);

    // Reading
    virtual string player_type() const;
    friend ostream& operator<<(ostream& os, const Player& player);
    string str() const;
    string repr() const;
};

#endif //PLAYER_HPP
