#include "pairs_arena.hpp"

int
PairsArena::get_column(int opponent, int prop) const {
    return opponent * (rules->player_count - 1) + prop - 1;
}

void
PairsArena::add_results(
    const std::vector<int>& ids,
    const std::vector<int>& new_wins,
    const std::vector<int>& new_scores,
    const std::vector<int>& new_squared_scores) {
    int first = ids.front();
    int second = ids.back();
    int prop = 1;
    while (ids[prop] == first) {
        prop++;
    }
    int index = 0;
    std::vector<int>* first_player_results = &results[first];
    int first_player_colum = get_column(second, prop);
    std::vector<int>* second_player_results = &results[second];
    int second_player_colum = get_column(first, rules->player_count - prop);
    for (int id : ids) {
        std::vector<int>* player_results =
            (id == first)
                ? first_player_results
                : second_player_results;
        int column =
            (id == first)
                ? first_player_colum
                : second_player_colum;
        (*player_results)[3 * column + 0] += new_wins[index];
        (*player_results)[3 * column + 1] += new_scores[index];
        (*player_results)[3 * column + 2] += new_squared_scores[index];
        index++;
    }
}

int
PairsArena::column_count() const {
    return 3 * (rules->player_count - 1) * players.size();
}

void
PairsArena::generate_groups(int available_players, int game_players) {
    for (int i = 0; i < available_players; i++) {
        for (int j = i + 1; j < available_players; j++) {
            for (int proportions = 1; proportions < game_players; proportions++) {
                std::vector<int> ids;
                for (int x = 0; x < game_players; x++) {
                    ids.push_back(x < proportions ? i : j);
                }
                add_group(ids);
            }
        }
    }
}


void
PairsArena::print_results(std::vector<std::vector<int>> results) {
    int player_count = results.size();
    int max_player_length = 6;
    int prop_count = rules->player_count - 1;
    for (int line = 0; line < player_count; line++) {
        max_player_length = std::max(max_player_length, int(players[line]->analysed_player->player_type().size()));
    }
    printf("Games per group:  %d\n\n", count);
    // 1 - Player line
    printf("%*s ", max_player_length, "player");
    for (int p = 0; p < players.size(); p++) {
        printf("| %*.*s ", 19 * prop_count, 19 * prop_count, players[p]->analysed_player->player_type().c_str());
    }
    printf("\n");
    // 2 - VS line
    printf("%*s ", max_player_length, "");
    for (int p = 0; p < players.size(); p++) {
        for (int c = 0; c < prop_count; c++) {
            printf("|       %dvs%d        ", c + 1, prop_count - c);
        }
    }
    printf("\n");
    // 3 - details line
    printf("%*s ", max_player_length, "");
    for (int l = 0; l < prop_count * players.size(); l++) {
        printf("|  win   avg   std  ");
    }
    printf("\n");
    // 4 - separation bar
    for (int i = 0; i <= max_player_length; i++) {
        printf("-");
    }
    for (int l = 0; l < prop_count * players.size(); l++) {
        printf("+------+-----+------");
    }
    printf("\n");
    // Body
    for (int line = 0; line < player_count; line++) {
        printf("%*.*s ",
            max_player_length,
            max_player_length,
            players[line]->analysed_player->player_type().c_str());
        for (int opponent = 0; opponent < players.size(); opponent++) {
            if (line == opponent) {
                for (int prop = 0; prop < prop_count; prop++) {
                    printf("|   ·     ·     ·   ");
                }
                continue;
            }

            for (int prop = 1; prop <= prop_count; prop++) {
                int column = get_column(opponent, prop);
                int games = count * prop;
                double winrate = 100. * results[line][3 * column + 0] / games;
                double avg_score = double(results[line][3 * column + 1]) / games;
                double score_var = std::sqrt(double(results[line][3 * column + 2]) / games - avg_score * avg_score);
                printf("| %4.1f%% %5.1f %5.1f ",
                    winrate,
                    avg_score,
                    score_var);
            }
        }
        printf("\n");
    }
    std::cout << std::flush;
}

std::string
PairsArena::mode_name() const {
    return "Pairs";
}

bool
PairsArena::ready() const {
    return players.size() >= 2;
}