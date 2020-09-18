#ifndef PAIRS_ARENA_HPP
#define PAIRS_ARENA_HPP

#include "arena.hpp"

class PairsArena : public Arena {
private:
    int get_column(int opponent, int prop) const;

protected:
    void virtual add_results(
        const vector<int>& ids,
        const vector<int>& new_wins,
        const vector<int>& new_scores,
        const vector<int>& new_squared_scores);

    virtual int column_count() const;
    virtual void generate_groups(int available_players, int game_players);
    virtual void print_results(vector<vector<int>> results);

public:
    using Arena::Arena;

    virtual string mode_name() const;

    bool virtual ready() const;
};

#endif //PAIRS_ARENA_HPP
