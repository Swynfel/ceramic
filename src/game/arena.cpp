#include "arena.hpp"

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
    // Setup all game groups
    while (!groups.empty()) {
        groups.pop();
    }
    vector<string> columns;
    switch (mode) {
        case Arena::MODE::EXACT:
            exact_groups();
            break;
        case Arena::MODE::SUBSET:
            subset_groups();
            break;
        case Arena::MODE::PAIRS:
            pairs_groups();
            break;
        case Arena::MODE::ALL:
            all_groups();
            break;
    }
    // Clear results
    int column_count = columns.size();
    results = vector<vector<int>>(player_count, vector<int>(column_count, 0));
    // TODO: Start threads
    // For now, just print list of groups to check it works as intended
    while (!groups.empty()) {
        for (int x : groups.front()) {
            std::cout << x << " ";
        }
        std::cout << std::endl;
        groups.pop();
    }
    // Print results
    for (int line = 0; line < player_count; line++) {
        std::cout << players[line]->player_type();
        for (int column = 0; column < column_count; column++) {
            std::cout << " | " << results[line][column];
        }
        std::cout << '\n';
    }
    std::cout << std::flush;
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