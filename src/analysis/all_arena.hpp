#ifndef ALL_ARENA_HPP
#define ALL_ARENA_HPP

#include "arena.hpp"

class AllArena : public Arena {
protected:
    virtual void generate_groups(int available_players, int game_players);

public:
    using Arena::Arena;

    virtual std::string mode_name() const;

    bool virtual ready() const;
};

#endif //ALL_ARENA_HPP
