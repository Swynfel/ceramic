#ifndef ARENA_HPP
#define ARENA_HPP

#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "analysis_player.hpp"
#include "game/game.hpp"
#include "game/player.hpp"
#include "rules/rules.hpp"

class Arena {
private:
    vector<thread> threads = {};

    queue<vector<int>> groups = {};

    std::mutex results_mutex = {};
    std::mutex queue_mutex = {};

    std::atomic<int> processed_steps{ 0 };
    std::atomic<int> processed_games{ 0 };
    int total_games;
    int total_steps;

    void add_results_container(
        const vector<int>& ids,
        const vector<int>& new_wins,
        const vector<int>& new_scores,
        const vector<int>& new_squared_scores);

    void run_single(vector<int> ids);

    void run_from_queue();
    void run_from_queue_async();

protected:
    vector<std::shared_ptr<AnalysisPlayer>> players;
    vector<vector<int>> results;

    std::atomic<long long> time{ 0 };

    void add_group(vector<int> group);

    void print_current();

    void virtual add_results(
        const vector<int>& ids,
        const vector<int>& new_wins,
        const vector<int>& new_scores,
        const vector<int>& new_squared_scores);

    virtual int column_count() const;
    virtual void generate_groups(int available_players, int game_players);
    virtual void print_results(vector<vector<int>> results);

public:
    int count = 1000;
    int thread_limit = 8;
    std::shared_ptr<Rules> rules;

    Arena();
    Arena(std::shared_ptr<Rules> rules, vector<std::shared_ptr<Player>> players = {});

    void add_player(std::shared_ptr<Player> player);
    void add_players(vector<std::shared_ptr<Player>> players);
    void remove_player(std::shared_ptr<Player> player);

    virtual string mode_name() const;

    bool virtual ready() const;
    void run();
};

#endif //ARENA_HPP
