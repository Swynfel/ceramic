#include <iostream>
#include <sstream>

#include <unistd.h>

#include "analysis/groups_utils.hpp"
#include "game/game.hpp"
#include "game/player.hpp"
#include "parsing.hpp"
#include "players/first_legal_player.hpp"
#include "players/random_player.hpp"
#include "players/terminal_player.hpp"
#include "utils/random.hpp"

void
print_help() {
    std::cout << "./ceramic-terminal-player [-h] [-c] [players] [-n] [-p p -m s|i|a]\n";
    std::cout << '\n';
    std::cout << "    -h : Help, shows this]\n";
    std::cout << "    -c f|g|n : Change color, use 'f' for full-color, 'g' for grey-scale, 'n' for none]\n";
    std::cout << '\n';
    PlayerParameters::help();
    std::cout << '\n';
    std::cout << "    -n 2-" << TILE_TYPES << " : Change number of tile types\n";
    std::cout << "    -g g : Play g games instead of 1\n";
    std::cout << "    -p p : Play again subsets of players of size p\n";
    std::cout << "    -m s|i|a : Mode to generate groups of size p (when -p is used)\n";
    std::cout << "        s : Subsets, take subsets of players\n";
    std::cout << "        i : Identical, take only groups with copies of the same player\n";
    std::cout << "        a : All, take all combinations (allows copy)\n";
    std::cout << '\n';
    std::cout << "In-game, enter -help form help" << std::endl;
}

enum OpponentMode {
    SUBSETS,
    IDENTICAL,
    ALL
};

struct GamesOptions {
    int player_limit;
    OpponentMode mode;
    int game_count;
};

bool
options(int argc, char* argv[], std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<TerminalPlayer>& terminal_player, std::shared_ptr<Rules>& rules, GamesOptions& options) {
    int option;
    while ((option = getopt(argc, argv, ":hc:n:p:m:g:")) != -1) { //get option from the getopt() method
        switch (option) {
            // Options
            case 'h':
                print_help();
                return false;
            case 'c':
                switch (optarg[0]) {
                    case 'f':
                        terminal_player->set_color_type(TerminalPlayer::ColoredType::FULL);
                        break;
                    case 'g':
                        terminal_player->set_color_type(TerminalPlayer::ColoredType::GREY);
                        break;
                    case 'n':
                        terminal_player->set_color_type(TerminalPlayer::ColoredType::NONE);
                        break;
                    default:
                        std::cout << "Unrecognised color argument: " << argv[1] << '\n';
                        std::cout << "Use 'f' for full-color, 'g' for grey-scale, 'n' for none" << std::endl;
                        return false;
                }
                break;
            // Rules
            case 'n':
                try {
                    int value = std::stoi(optarg);
                    if (value < 2 || value > TILE_TYPES) {
                        throw std::out_of_range("Wrong tile count");
                    }
                    rules->tile_types = value;
                } catch (const std::exception& e) {
                    std::cout << "Unrecognised tile types cout: " << optarg << '\n';
                    std::cout << "Use an int between 2 and " << TILE_TYPES << " (included)" << std::endl;
                    return false;
                }
                break;
            // Player count
            case 'p':
                try {
                    int value = std::stoi(optarg);
                    options.player_limit = value;
                } catch (const std::exception& e) {
                    std::cout << "Unrecognised player limit: " << optarg << '\n';
                    return false;
                }
                break;
            // Mode
            case 'm':
                switch (optarg[0]) {
                    case 's':
                        options.mode = OpponentMode::SUBSETS;
                        break;
                    case 'i':
                        options.mode = OpponentMode::IDENTICAL;
                        break;
                    case 'a':
                        options.mode = OpponentMode::ALL;
                        break;
                    default:
                        std::cout << "Unrecognised mode argument: " << argv[1] << '\n';
                        std::cout << "Use 's' for subsets, 'i' for identical, 'a' for all" << std::endl;
                        return false;
                }
                break;
            // Games
            case 'g':
                try {
                    int value = std::stoi(optarg);
                    options.game_count = value;
                } catch (const std::exception& e) {
                    std::cout << "Unrecognised game count: " << optarg << '\n';
                    return false;
                }
                break;
            // Errors
            case ':':
                std::cout << "Missing argument for option " << optopt << '\n';
                print_help();
                return false;
            case '?':
                std::cout << "Unkown option " << optopt << '\n';
                print_help();
                return false;
        }
    }
    for (; optind < argc; optind++) {
        std::string arg = argv[optind];
        try {
            players.push_back(PlayerParameters::parse(arg).build());
        } catch (const std::exception& e) {
            std::cout << e.what() << '\n'
                      << "Error parsing input, use '-h' for help\n"
                      << std::endl;
            throw e;
        }
    }
    if (players.size() == 0) {
        players = {
            std::make_shared<FirstLegalPlayer>(),
            std::make_shared<RandomPlayer>(false),
            std::make_shared<RandomPlayer>()
        };
    }
    return true;
}

void
save(std::ostringstream& os, const Game& game) {
    const State& state = game.get_state();
    for (ushort p = 0U; p < state.get_rules()->player_count; p++) {
        os << game.get_player_at(p)->player_type() << " " << state.get_panel(p).get_score() << " ";
    }
    int winner = state.winning_player();
    os << game.get_player_at(winner)->player_type() << " " << winner << "\n";
}

int
main(int argc, char* argv[]) {
    std::vector<std::shared_ptr<Player>> players;
    std::shared_ptr<TerminalPlayer> terminal_player = std::make_shared<TerminalPlayer>();
    std::shared_ptr<Rules> rules = std::make_shared<Rules>(*Rules::BASE);
    GamesOptions gamesOptions{ .player_limit = -1, .mode = SUBSETS, .game_count = 1 };
    if (!options(argc, argv, players, terminal_player, rules, gamesOptions)) {
        return 1;
    }
    // Game
    std::ostringstream os;
    if (gamesOptions.player_limit < 0) {
        rules->player_count = 1 + players.size();
        Game game = Game(rules);
        game.add_player(terminal_player);
        game.add_players(players);
        try {
            for (int total = gamesOptions.game_count; total > 0; total--) {
                std::cout << "Game left " << total << std::endl;
                game.roll_game();
                save(os, game);
            }
        } catch (std::exception& e) {
        }
    } else {
        rules->player_count = 1 + gamesOptions.player_limit;
        std::queue<std::vector<int>> groups{};
        switch (gamesOptions.mode) {
            case SUBSETS:
                GroupUtils::subsets(groups, players.size(), gamesOptions.player_limit);
                break;
            case IDENTICAL:
                GroupUtils::identical(groups, players.size(), gamesOptions.player_limit);
                break;
            case ALL:
                GroupUtils::all(groups, players.size(), gamesOptions.player_limit, false);
                break;
        }
        std::vector<std::pair<int, std::vector<int>>> possibleGames{};
        int total = groups.size() * gamesOptions.game_count;
        while (!groups.empty()) {
            std::vector<int> group = groups.front();
            possibleGames.push_back(std::pair<int, std::vector<int>>(gamesOptions.game_count, group));
            groups.pop();
        }
        rng randomness(random_seed());
        ushort_range range{};
        while (total > 0) {
            // Finding group
            int i = random_range(randomness, range, 0, total);
            std::vector<int> group;
            for (size_t g = 0; g < possibleGames.size(); g++) {
                i -= possibleGames[g].first;
                if (i < 0) {
                    possibleGames[g].first--;
                    group = possibleGames[g].second;
                    break;
                }
            }
            // Running game
            std::cout << "Game left " << total << std::endl;
            Game game = Game(rules);
            game.add_player(terminal_player);
            for (int id : group) {
                game.add_player(players[id]);
            }
            try {
                game.roll_game();
                save(os, game);
            } catch (std::exception& e) {
                break;
            }
            total--;
        }
    }
    std::cout << "Results :\n";
    std::cout << os.str() << std::endl;
}
