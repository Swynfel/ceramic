#include "arena.hpp"

#include "arena_room.hpp"
#include "groups_utils.hpp"
#include <cmath>
#include <stdio.h>

/*** Private ***/

void
Arena::add_results_container(
    std::vector<std::vector<int>>& _results,
    const std::vector<int>& ids,
    const std::vector<int>& new_wins,
    const std::vector<int>& new_scores,
    const std::vector<int>& new_squared_scores) {
    const std::lock_guard<std::mutex> lock(results_mutex);
    add_results(_results, ids, new_wins, new_scores, new_squared_scores);
    processed_groups++;
    print_current();
}

bool
Arena::is_sequential() const {
    return thread_limit <= 1;
}

/*** Protected ***/

void
Arena::add_group(std::vector<int> group) {
    groups.push(std::move(group));
}

void
Arena::print_current() {
    std::cout << "Played " << processed_groups << "/" << total_groups << " (" << processed_games << "/" << total_games << ")    \r" << std::flush;
}


// Default protected virtual methods

void
Arena::add_results(
    std::vector<std::vector<int>>& _results,
    const std::vector<int>& ids,
    const std::vector<int>& new_wins,
    const std::vector<int>& new_scores,
    const std::vector<int>& new_squared_scores) {
    for (size_t i = 0; i < ids.size(); i++) {
        std::vector<int>& player_results = _results[ids[i]];
        player_results[0]++;
        player_results[1] += new_wins[i];
        player_results[2] += new_scores[i];
        player_results[3] += new_squared_scores[i];
    }
}


int
Arena::column_count() const {
    return 4;
}

void
Arena::generate_groups(int available_players, int game_players) {
    GroupUtils::subsets(groups, available_players, game_players);
}

void
Arena::print_results(std::vector<std::vector<int>> results) {
    int player_count = results.size();
    int max_player_length = 6;
    for (int line = 0; line < player_count; line++) {
        max_player_length = std::max(max_player_length, int(players[line]->analysed_player->player_type().size()));
    }
    int games_per_player = results[0][0] * count;

    printf("Games per group:  %d\nGames per player: %d\n", count, games_per_player);

    double game_time = process_time / total_games;
    if (detailed_player_analysis) {
        double state_change_time = process_time;
        int total_moves = 0;
        for (auto& player : players) {
            state_change_time -= player->time;
            total_moves += player->move_counter;
        }
        printf("Total time: %.4e µs (real), %.4e (per thread)", (double)real_time, (double)real_time / (double)thread_limit);
        printf("Time: %.3e µs (game), %.3e µs (step), %.3e µs (state change)\nAverage moves per game: %.1f\n\n",
            game_time,
            (double)process_time / total_moves,
            state_change_time / total_moves,
            (double)total_moves / total_games);
    } else {
        printf("Average time per game: %.4e µs\n\n", game_time);
    }

    // Table
    printf("%*s | winrate |  avg  |  std  %s\n", max_player_length, "player", detailed_player_analysis ? "| move time |moves" : "");
    for (int i = 0; i < max_player_length; i++) {
        printf("-");
    }
    printf("-+---------+-------+-------%s\n", detailed_player_analysis ? "+-----------+-----" : "");
    for (int line = 0; line < player_count; line++) {
        double winrate = 100. * results[line][1] / games_per_player;
        double avg_score = double(results[line][2]) / games_per_player;
        double score_var = std::sqrt(double(results[line][3]) / games_per_player - avg_score * avg_score);
        if (detailed_player_analysis) {
            double time_per_move = players[line]->time * 1.0 / players[line]->move_counter;
            double moves = (double)players[line]->move_counter / games_per_player;
            printf("%*.*s | %6.2f%% | %5.1f | %5.1f | %.3e |% 3.1f\n",
                max_player_length,
                max_player_length,
                players[line]->analysed_player->player_type().c_str(),
                winrate,
                avg_score,
                score_var,
                time_per_move,
                moves);
        } else {
            printf("%*.*s | %6.2f%% | %5.1f | %5.1f\n",
                max_player_length,
                max_player_length,
                players[line]->analysed_player->player_type().c_str(),
                winrate,
                avg_score,
                score_var);
        }
    }
    std::cout << std::flush;
}


/*** Public ***/

Arena::Arena()
  : Arena(std::make_shared<Rules>(*Rules::DEFAULT)) {}

Arena::Arena(std::shared_ptr<Rules> rules, std::vector<std::shared_ptr<Player>> players)
  : players()
  , rules(rules) {
    for (std::shared_ptr<Player> player : players) {
        add_player(std::move(player));
    }
}


void
Arena::add_player(std::shared_ptr<Player> player) {
    players.push_back(std::make_shared<AnalysisPlayer>(std::move(player)));
}

void
Arena::add_players(std::vector<std::shared_ptr<Player>> players) {
    for (auto player : players) {
        add_player(std::move(player));
    }
}

void
Arena::remove_player(std::shared_ptr<Player> player) {
    auto it = players.begin();
    while (it != players.end()) {
        if ((*it)->analysed_player == player) {
            players.erase(it);
            return;
        }
    }
}


std::string
Arena::mode_name() const {
    return "Subsets";
}

bool
Arena::ready() const {
    return players.size() >= rules->player_count;
}

void
Arena::run() {
    auto begin_instant = std::chrono::high_resolution_clock::now();
    int player_count = players.size();
    // Check
    if (!ready()) {
        throw std::runtime_error("Arena not ready: missing player(s)");
    }
    if (thread_limit <= 0) {
        throw std::runtime_error("Thread_limit should be strictly positive");
    }
    std::cout << "Mode: " << mode_name() << "\n";
    // Setup all game groups
    while (!groups.empty()) {
        groups.pop();
    }
    generate_groups(players.size(), rules->player_count);
    // Clear results
    process_time = 0;
    processed_groups = 0;
    processed_games = 0;
    total_groups = groups.size();
    total_games = total_groups * count;
    results = std::vector<std::vector<int>>(player_count, std::vector<int>(column_count(), 0));
    for (auto& player : players) {
        player->time = 0;
        player->move_counter = 0;
        player->analysis = detailed_player_analysis;
    }
    // Run games
    std::vector<std::thread> rooms = std::vector<std::thread>();
    print_current();
    for (int i = 0; i < thread_limit; i++) {
        rooms.push_back(std::thread(&ArenaRoom::run, ArenaRoom(this)));
    }
    for (auto& room : rooms) {
        room.join();
    }
    auto end_instant = std::chrono::high_resolution_clock::now();
    real_time = std::chrono::duration_cast<std::chrono::microseconds>(end_instant - begin_instant).count();
    std::cout << std::endl;
    // Print results
    print_results(std::move(results));
}