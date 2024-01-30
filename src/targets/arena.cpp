#include <iostream>

#include <memory>
#include <unistd.h>

#include "analysis/all_arena.hpp"
#include "analysis/arena.hpp"
#include "analysis/pairs_arena.hpp"
#include "players/first_legal_player.hpp"
#include "players/monte_carlo_player.hpp"
#include "players/random_player.hpp"

#include "parsing.hpp"

void
print_help() {
    std::cout << "./ceramic-arena [-h] <players> [-a <arena_type>] [-n <tile_count>] [-p <player_count>] [-g <game_per_group>] [-t <thread_limit>] [-z]\n";
    std::cout << '\n';
    std::cout << "    -h : Help, shows this]\n";
    std::cout << '\n';
    PlayerParameters::help();
    std::cout << '\n';
    std::cout << "    -a <arena_type>\n"
              << "        s : subsets\n"
              << "        p : pairs (default)\n"
              << "        a : all\n"
              << '\n';
    std::cout << "    -n <tile_count> : int (default is 5)\n"
              << "    -p <player_count> : int (default is 4)\n"
              << "    -g <game_per_group> : int (default is 1000)\n"
              << "    -t <thread_limit> : int (default is 8)\n"
              << "    -z : deactivate detailed player analysis\n";
    std::cout << std::endl;
}

struct ArenaOptions {
    int count;
    int thread_limit;
    bool detailed_player_analysis;
};

enum ArenaMode {
    SUBSETS,
    PAIRS,
    ALL,
};

bool
options(int argc, char* argv[], std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Rules>& rules, ArenaMode& arena_mode, ArenaOptions& arena_options) {
    int option;
    while ((option = getopt(argc, argv, ":hc:n:p:a:g:t:z")) != -1) { // get option from the getopt() method
        switch (option) {
            // Help
            case 'h':
                print_help();
                return false;
            // Arena
            case 'a':
                switch (optarg[0]) {
                    case 's':
                        arena_mode = ArenaMode::SUBSETS;
                        break;
                    case 'p':
                        arena_mode = ArenaMode::PAIRS;
                        break;
                    case 'a':
                        arena_mode = ArenaMode::ALL;
                        break;
                    default:
                        std::cout << "Unrecognised arena argument: " << optarg << '\n';
                        std::cout << "Use 's' for subsets, 'p' pairs, 'a' for all" << std::endl;
                        return false;
                }
                break;
            case 'g':
                arena_options.count = std::stoi(optarg);
                break;
            case 't':
                arena_options.thread_limit = std::stoi(optarg);
                break;
            case 'z':
                arena_options.detailed_player_analysis = false;
                break;
            // Rules
            case 'n':
                try {
                    int value = std::stoi(optarg);
                    if (value < 2 || value > TILE_TYPES) {
                        throw std::out_of_range("");
                    }
                    rules->tile_types = value;
                } catch (const std::exception& e) {
                    std::cout << "Unrecognised tile types count: " << optarg << '\n';
                    std::cout << "Use an integer between 2 and " << TILE_TYPES << " (included)" << std::endl;
                    return false;
                }
                break;
            case 'p':
                try {
                    int value = std::stoi(optarg);
                    if (value < 2) {
                        throw std::out_of_range("");
                    }
                    rules->player_count = value;
                } catch (const std::exception& e) {
                    std::cout << "Unrecognised player count: " << optarg << '\n';
                    std::cout << "Use an integer greater or equals to 2" << std::endl;
                    return false;
                }
                break;
            // Errors
            case ':':
                std::cout << "Missing argument for option -" << char(optopt) << '\n';
                print_help();
                return false;
            case '?':
                std::cout << "Unkown option -" << char(optopt) << '\n';
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

int
main(int argc, char* argv[]) {
    std::vector<std::shared_ptr<Player>> players;
    std::shared_ptr<Rules> rules = std::make_shared<Rules>(*Rules::BASE);
    ArenaMode arena_mode = ArenaMode::ALL;
    ArenaOptions arena_options{ .count = 1000, .thread_limit = 8, .detailed_player_analysis = true };
    if (!options(argc, argv, players, rules, arena_mode, arena_options)) {
        return 1;
    }
    std::unique_ptr<Arena> arena;
    switch (arena_mode) {
        case ArenaMode::SUBSETS:
            arena = std::make_unique<Arena>(rules, players);
            break;
        case ArenaMode::PAIRS:
            arena = std::make_unique<PairsArena>(rules, players);
            break;
        case ArenaMode::ALL:
            arena = std::make_unique<AllArena>(rules, players);
            break;
        default:
            throw std::runtime_error("Unkown Arena Mode");
    }
    arena->count = arena_options.count;
    arena->thread_limit = arena_options.thread_limit;
    arena->detailed_player_analysis = arena_options.detailed_player_analysis;
    arena->run_print();
}
