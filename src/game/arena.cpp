#include "arena.hpp"

#include <cmath>
#include <stdio.h>

Arena::Arena()
  : Arena(std::make_shared<Rules>(*Rules::DEFAULT)) {}

Arena::Arena(std::shared_ptr<Rules> rules, vector<std::shared_ptr<Player>> players)
  : players()
  , threads()
  , games()
  , rules(rules) {
    for (std::shared_ptr<Player> player : players) {
        add_player(std::move(player));
    }
}

void
Arena::add_results(
    const vector<int>& ids,
    const vector<int>& new_count,
    const vector<int>& new_wins,
    const vector<int>& new_scores,
    const vector<int>& new_squared_scores) {
    const std::lock_guard<std::mutex> lock(results_mutex);
    if (mode == Arena::MODE::PAIRS) {
        throw std::logic_error("Mode Pairs not implemented");
    }
    for (int i = 0; i < ids.size(); i++) {
        vector<int>& player_results = results[ids[i]];
        player_results[0] += new_count[i];
        player_results[1] += new_wins[i];
        player_results[2] += new_scores[i];
        player_results[3] += new_squared_scores[i];
    }
    processed_groups++;
    print_current();
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
Arena::mode_name() {
    switch (mode) {
        case Arena::MODE::EXACT:
            return "Exact";
        case Arena::MODE::SUBSET:
            return "Subset";
        case Arena::MODE::PAIRS:
            return "Pairs";
        case Arena::MODE::ALL:
            return "All";
        default:
            throw runtime_error("Unknown Mode");
    }
}


bool
Arena::ready() const {
    switch (mode) {
        case Arena::MODE::EXACT:
            return players.size() == rules->player_count;

        case Arena::MODE::SUBSET:
            return players.size() >= rules->player_count;

        default:
            return players.size() >= 2;
    }
}

void
Arena::run() {
    int player_count = players.size();
    // Check
    switch (mode) {
        case Arena::MODE::EXACT:
            if (player_count > rules->player_count) {
                throw runtime_error("Too many players for exact");
            }
        case Arena::MODE::SUBSET:
            if (player_count < rules->player_count) {
                throw runtime_error("Not enough players");
            }
            break;
        default:
            if (player_count < 2) {
                throw runtime_error("Needs at least two players");
            }
    }
    std::cout << "Mode: " << mode_name() << "\n";
    // Setup all game groups
    while (!groups.empty()) {
        groups.pop();
    }
    int column_count;
    switch (mode) {
        case Arena::MODE::EXACT:
            column_count = 4;
            exact_groups();
            break;
        case Arena::MODE::SUBSET:
            column_count = 4;
            subset_groups();
            break;
        case Arena::MODE::PAIRS:
            column_count = 0;
            pairs_groups();
            break;
        case Arena::MODE::ALL:
            column_count = 4;
            all_groups();
            break;
    }
    processed_groups = 0;
    total_groups = groups.size();
    // Clear results
    results = vector<vector<int>>(player_count, vector<int>(column_count, 0));
    // Start threads
    threads.clear();
    for (int i = 0; i < thread_limit; i++) {
        threads.push_back(thread(&Arena::run_from_queue_async, this));
    }
    print_current();
    for (auto& t : threads) {
        t.join();
    }
    std::cout << std::endl;
    // Print results
    switch (mode) {
        case Arena::MODE::PAIRS:
            pairs_results();
            break;
        default:
            default_results();
            break;
    }
}


void
Arena::print_current() {
    std::cout << "Played " << processed_groups << "/" << total_groups << '\r' << std::flush;
}


void
Arena::run_single(vector<int> ids) {
    int p = ids.size();
    Game game = Game(rules);
    for (int id : ids) {
        game.add_player(players[id]);
    }
    vector<int> play_count(p, count);
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
    add_results(ids, play_count, win_count, score_sum, squared_score_sum);
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


void
Arena::exact_groups() {
    vector<int> ids;
    for (int x = 0; x < rules->player_count; x++) {
        ids.push_back(x);
    }
    groups.push(ids);
}

void
Arena::subset_groups() {
    const int N = rules->player_count;
    const int M = players.size();
    vector<int> ids;
    ids.push_back(0);
    while (true) {
        if (ids.back() >= M) {
            ids.pop_back();
            if (ids.empty()) {
                return;
            }
            ids.back()++;
            continue;
        }
        if (ids.size() < N) {
            ids.push_back(ids.back() + 1);
            continue;
        }
        groups.push(ids);
        ids.back()++;
    }
}

void
Arena::pairs_groups() {
    for (int i = 0; i < rules->player_count; i++) {
        for (int j = i + 1; j < rules->player_count; j++) {
            for (int proportions = 1; proportions < rules->player_count; proportions++) {
                vector<int> ids;
                for (int x = 0; x < rules->player_count; x++) {
                    ids.push_back(x < proportions ? i : j);
                }
                groups.push(ids);
            }
        }
    }
}

void
Arena::all_groups() {
    const int N = rules->player_count;
    const int M = players.size();
    vector<int> ids;
    ids.push_back(0);
    while (true) {
        if (ids.back() >= M) {
            ids.pop_back();
            if (ids.empty()) {
                return;
            }
            ids.back()++;
            continue;
        }
        if (ids.size() < N) {
            ids.push_back(ids.back());
            continue;
        }
        if (ids[0] != ids[N - 1]) {
            groups.push(ids);
        }
        ids.back()++;
    }
}

void
Arena::default_results() {
    int player_count = players.size();
    int max_player_length = 6;
    for (int line = 0; line < player_count; line++) {
        max_player_length = max(max_player_length, int(players[line]->player_type().size()));
    }
    int games_per_player = results[0][0];
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

void
Arena::pairs_results() {
    throw logic_error("Not implemented");
}