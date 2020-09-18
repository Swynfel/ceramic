#include "all_arena.hpp"

void
AllArena::generate_groups(int available_players, int game_players) {
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
            ids.push_back(ids.back());
            continue;
        }
        if (ids[0] != ids[game_players - 1]) {
            add_group(ids);
        }
        ids.back()++;
    }
}


string
AllArena::mode_name() const {
    return "All";
}

bool
AllArena::ready() const {
    return players.size() >= 2;
}