#include "arena_room.hpp"

ArenaRoom::ArenaRoom(Arena* arena)
  : arena(arena)
  , players()
  , results(arena->players.size(), std::vector<int>(arena->column_count(), 0))
  , execution_time(0LL) {
    for (auto it = arena->players.begin(); it != arena->players.end(); it++) {
        players.push_back(std::static_pointer_cast<AnalysisPlayer>((*it)->copy()));
    }
}

ArenaRoom::ArenaRoom(const ArenaRoom& room)
  : arena(room.arena)
  , players(room.players)
  , results(room.results)
  , execution_time(room.execution_time) {}

void
ArenaRoom::run_single(std::vector<int> ids) {
    int p = ids.size();
    Game game = Game(arena->rules);
    for (int id : ids) {
        game.add_player(players[id]);
    }
    std::vector<int> win_count(p, 0);
    std::vector<int> score_sum(p, 0);
    std::vector<int> squared_score_sum(p, 0);
    for (int c = 0; c < arena->count; c++) {
        auto begin = std::chrono::high_resolution_clock::now();
        game.roll_game();
        auto end = std::chrono::high_resolution_clock::now();
        int winner = game.order[game.state.winning_player()];
        win_count[winner] += 1;
        for (int position = 0; position < p; position++) {
            int id = game.order[position];
            int score = game.state.get_panel(position).get_score();
            score_sum[id] += score;
            squared_score_sum[id] += score * score;
        }
        execution_time += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        arena->processed_games++;
        arena->print_current();
    }
    arena->add_results(results, ids, win_count, score_sum, squared_score_sum);
    arena->processed_groups++;
    arena->print_current();
}

void
ArenaRoom::run_sequential() {
    while (!arena->groups.empty()) {
        run_single(arena->groups.front());
        arena->groups.pop();
    }
}

void
ArenaRoom::run_async() {
    while (!arena->groups.empty()) {
        std::vector<int> ids;
        {
            const std::lock_guard<std::mutex> lock(arena->queue_mutex);
            if (arena->groups.empty()) {
                break;
            }
            ids = arena->groups.front();
            arena->groups.pop();
        }
        run_single(ids);
    }
}

void
ArenaRoom::run() {
    arena->is_sequential() ? run_sequential() : run_async();
    // Send data back to arena
    const std::lock_guard<std::mutex> lock(arena->results_mutex);
    int player = 0;
    for (auto local_player = players.begin(); local_player != players.end(); local_player++) {
        arena->players[player]->time += (*local_player)->time;
        arena->players[player]->move_counter += (*local_player)->move_counter;
        int column = 0;
        for (auto local_line = results[player].begin(); local_line != results[player].end(); local_line++) {
            arena->results[player][column] += *local_line;
            column++;
        }
        player++;
    }
    arena->time += execution_time;
}