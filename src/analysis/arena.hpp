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
    std::vector<std::thread> threads = {};

    std::queue<std::vector<int>> groups = {};

    std::mutex results_mutex = {};
    std::mutex queue_mutex = {};

    std::atomic<int> processed_games{ 0 };
    std::atomic<int> processed_groups{ 0 };
    int total_groups;
    int total_games;

    void add_results_container(
        const std::vector<int>& ids,
        const std::vector<int>& new_wins,
        const std::vector<int>& new_scores,
        const std::vector<int>& new_squared_scores);

    void run_single(std::vector<int> ids);

    void run_from_queue();
    void run_from_queue_async();

protected:
    std::vector<std::shared_ptr<AnalysisPlayer>> players;
    std::vector<std::vector<int>> results;

    std::atomic<long long> time{ 0 };

    void add_group(std::vector<int> group);

    void print_current();

    void virtual add_results(
        const std::vector<int>& ids,
        const std::vector<int>& new_wins,
        const std::vector<int>& new_scores,
        const std::vector<int>& new_squared_scores);

    virtual int column_count() const;
    virtual void generate_groups(int available_players, int game_players);
    virtual void print_results(std::vector<std::vector<int>> results);

public:
    int count = 1000;
    int thread_limit = 8;
    bool detailed_player_analysis = true;
    std::shared_ptr<Rules> rules;

    Arena();
    Arena(std::shared_ptr<Rules> rules, std::vector<std::shared_ptr<Player>> players = {});

    void add_player(std::shared_ptr<Player> player);
    void add_players(std::vector<std::shared_ptr<Player>> players);
    void remove_player(std::shared_ptr<Player> player);

    virtual std::string mode_name() const;

    bool virtual ready() const;
    void run();
};

#endif //ARENA_HPP
