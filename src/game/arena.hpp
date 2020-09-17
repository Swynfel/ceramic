#ifndef ARENA_HPP
#define ARENA_HPP

#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "game/game.hpp"
#include "player.hpp"
#include "rules/rules.hpp"

class Arena {
private:
    vector<std::shared_ptr<Player>> players;
    vector<thread> threads;
    vector<Game> games;

    queue<vector<int>> groups;
    vector<vector<int>> results;

    std::mutex results_mutex = {};
    std::mutex queue_mutex = {};

    void add_results(
        const vector<int>& ids,
        const vector<int>& new_count,
        const vector<int>& new_wins,
        const vector<int>& new_scores,
        const vector<int>& new_squared_scores);

public:
    enum MODE {
        EXACT,
        SUBSET,
        PAIRS,
        ALL,
    };

    Arena::MODE mode = Arena::MODE::EXACT;
    int count = 1000;
    int thread_limit = 8;
    std::shared_ptr<Rules> rules;

    Arena();
    Arena(std::shared_ptr<Rules> rules, vector<std::shared_ptr<Player>> players = {});

    void add_player(std::shared_ptr<Player> player);
    void add_players(vector<std::shared_ptr<Player>> players);
    void remove_player(std::shared_ptr<Player> player);

    bool ready() const;
    void run();

private:
    void run_from_queue();
    void run_from_queue_async();
    void run_single(vector<int> ids);
    void exact_groups();
    void subset_groups();
    void pairs_groups();
    void all_groups();
};

#endif //ARENA_HPP
