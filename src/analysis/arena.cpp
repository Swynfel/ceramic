#include "arena.hpp"

#include <cmath>
#include <stdio.h>

/*** Private ***/

void
Arena::add_results_container(
    const vector<int>& ids,
    const vector<int>& new_wins,
    const vector<int>& new_scores,
    const vector<int>& new_squared_scores) {
    const std::lock_guard<std::mutex> lock(results_mutex);
    add_results(ids, new_wins, new_scores, new_squared_scores);
    print_current();
}

void
Arena::run_single(vector<int> ids) {
    int p = ids.size();
    Game game = Game(rules);
    for (int id : ids) {
        game.add_player(players[id]);
    }
    vector<int> win_count(p, 0);
    vector<int> score_sum(p, 0);
    vector<int> squared_score_sum(p, 0);
    for (int c = 0; c < count; c++) {
        game.roll_game();
        int winner = game.order[game.state.winning_player()];
        win_count[winner] += 1;
        for (int position = 0; position < p; position++) {
            int id = game.order[position];
            int score = game.state.get_panel(position).get_score();
            score_sum[id] += score;
            squared_score_sum[id] += score * score;
        }
    }
    add_results(ids, win_count, score_sum, squared_score_sum);
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
        vector<int> ids;
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
Arena::add_group(vector<int> group) {
    groups.push(std::move(group));
}

void
Arena::print_current() {
    std::cout << "Played " << processed_steps << "/" << total_steps << '\r' << std::flush;
}


// Default protected virtual methods

void
Arena::add_results(
    const vector<int>& ids,
    const vector<int>& new_wins,
    const vector<int>& new_scores,
    const vector<int>& new_squared_scores) {
    for (int i = 0; i < ids.size(); i++) {
        vector<int>& player_results = results[ids[i]];
        player_results[0]++;
        player_results[1] += new_wins[i];
        player_results[2] += new_scores[i];
        player_results[3] += new_squared_scores[i];
    }
    processed_steps++;
}


int
Arena::column_count() const {
    return 4;
}

void
Arena::generate_groups(int available_players, int game_players) {
    vector<int> ids;
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
Arena::print_results(vector<vector<int>> results) {
    int player_count = results.size();
    int max_player_length = 6;
    for (int line = 0; line < player_count; line++) {
        max_player_length = max(max_player_length, int(players[line]->player_type().size()));
    }
    int games_per_player = results[0][0] * count;
    printf("Games per group:  %d\nGames per player: %d\n\n", count, games_per_player);
    printf("%*s | winrate |  avg  |  std  \n", max_player_length, "player");
    for (int i = 0; i < max_player_length; i++) {
        printf("-");
    }
    printf("-+---------+-------+-------\n");
    for (int line = 0; line < player_count; line++) {
        double winrate = 100. * results[line][1] / games_per_player;
        double avg_score = double(results[line][2]) / games_per_player;
        double score_var = std::sqrt(double(results[line][3]) / games_per_player - avg_score * avg_score);
        printf("%*.*s | %5.2f %% | %5.1f | %5.1f\n",
            max_player_length,
            max_player_length,
            players[line]->player_type().c_str(),
            winrate,
            avg_score,
            score_var);
    }
    std::cout << std::flush;
}


/*** Public ***/

Arena::Arena()
  : Arena(std::make_shared<Rules>(*Rules::DEFAULT)) {}

Arena::Arena(std::shared_ptr<Rules> rules, vector<std::shared_ptr<Player>> players)
  : players()
  , rules(rules) {
    for (std::shared_ptr<Player> player : players) {
        add_player(std::move(player));
    }
}


void
Arena::add_player(std::shared_ptr<Player> player) {
    players.push_back(std::move(player));
}

void
Arena::add_players(vector<std::shared_ptr<Player>> players) {
    for (auto player : players) {
        add_player(std::move(player));
    }
}

void
Arena::remove_player(std::shared_ptr<Player> player) {
    auto it = std::find(players.begin(), players.end(), player);
    if (it != players.end()) {
        players.erase(it);
    }
}


string
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
        throw runtime_error("Arena not ready");
    }
    std::cout << "Mode: " << mode_name() << "\n";
    // Setup all game groups
    while (!groups.empty()) {
        groups.pop();
    }
    generate_groups(players.size(), rules->player_count);
    // Clear results
    processed_steps = 0;
    total_steps = groups.size();
    results = vector<vector<int>>(player_count, vector<int>(column_count(), 0));
    // Run games
    print_current();
    if (thread_limit > 1) {
        // Asynchronously
        threads.clear();
        for (int i = 0; i < thread_limit; i++) {
            threads.push_back(thread(&Arena::run_from_queue_async, this));
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