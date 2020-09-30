#include "arena.hpp"

#include <cmath>
#include <stdio.h>

/*** Private ***/

void
Arena::add_results_container(
    const std::vector<int>& ids,
    const std::vector<int>& new_wins,
    const std::vector<int>& new_scores,
    const std::vector<int>& new_squared_scores) {
    const std::lock_guard<std::mutex> lock(results_mutex);
    add_results(ids, new_wins, new_scores, new_squared_scores);
    processed_groups++;
    print_current();
}

void
Arena::run_single(std::vector<int> ids) {
    int p = ids.size();
    Game game = Game(rules);
    for (int id : ids) {
        game.add_player(players[id]);
    }
    std::vector<int> win_count(p, 0);
    std::vector<int> score_sum(p, 0);
    std::vector<int> squared_score_sum(p, 0);
    for (int c = 0; c < count; c++) {
        auto begin = std::chrono::high_resolution_clock::now();
        game.roll_game();
        auto end = std::chrono::high_resolution_clock::now();
        // std::cout << "FINAL STATE\n"
        //           << game.get_state() << '\n';
        int winner = game.order[game.state.winning_player()];
        win_count[winner] += 1;
        for (int position = 0; position < p; position++) {
            int id = game.order[position];
            int score = game.state.get_panel(position).get_score();
            score_sum[id] += score;
            squared_score_sum[id] += score * score;
        }
        processed_games++;
        time.fetch_add(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
        print_current();
    }
    add_results_container(ids, win_count, score_sum, squared_score_sum);
}


void
Arena::run_from_queue() {
    while (!groups.empty()) {
        run_single(groups.front());
        groups.pop();
    }
}

void
Arena::run_from_queue_async() {
    while (!groups.empty()) {
        std::vector<int> ids;
        {
            const std::lock_guard<std::mutex> lock(queue_mutex);
            if (groups.empty()) {
                break;
            }
            ids = groups.front();
            groups.pop();
        }
        run_single(ids);
    }
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
    const std::vector<int>& ids,
    const std::vector<int>& new_wins,
    const std::vector<int>& new_scores,
    const std::vector<int>& new_squared_scores) {
    for (int i = 0; i < ids.size(); i++) {
        std::vector<int>& player_results = results[ids[i]];
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
    std::vector<int> ids;
    ids.push_back(0);
    while (true) {
        if (ids.back() >= available_players) {
            ids.pop_back();
            if (ids.empty()) {
                return;
            }
            ids.back()++;
            continue;
        }
        if (ids.size() < game_players) {
            ids.push_back(ids.back() + 1);
            continue;
        }
        groups.push(ids);
        ids.back()++;
    }
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

    double game_time = time / total_games;
    if (detailed_player_analysis) {
        double state_change_time = time;
        int total_moves = 0;
        for (auto& player : players) {
            state_change_time -= player->time;
            total_moves += player->move_counter;
        }
        printf("Time: %.3e µs (game), %.3e µs (step), %.3e µs (state change)\nAverage moves per game: %.1f\n\n",
            (double)time / total_games,
            (double)time / total_moves,
            state_change_time / total_moves,
            (double)total_moves / total_games);
    } else {
        printf("Average time per game: %.4e µs\n\n",
            (double)time / total_games);
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
    int player_count = players.size();
    // Check
    if (!ready()) {
        throw std::runtime_error("Arena not ready");
    }
    std::cout << "Mode: " << mode_name() << "\n";
    // Setup all game groups
    while (!groups.empty()) {
        groups.pop();
    }
    generate_groups(players.size(), rules->player_count);
    // Clear results
    time = 0;
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
    print_current();
    if (thread_limit > 1) {
        // Asynchronously
        threads.clear();
        for (int i = 0; i < thread_limit; i++) {
            threads.push_back(std::thread(&Arena::run_from_queue_async, this));
        }
        for (auto& t : threads) {
            t.join();
        }
    } else {
        // Sequentially
        run_from_queue();
    }
    std::cout << std::endl;
    // Print results
    print_results(std::move(results));
}