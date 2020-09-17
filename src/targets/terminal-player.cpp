#include <iostream>

#include <unistd.h>

#include "game/game.hpp"
#include "game/player.hpp"
#include "players/first_legal_player.hpp"
#include "players/random_player.hpp"
#include "players/terminal_player.hpp"

void
print_help_players() {
    std::cout << "    r : Random Player]\n";
    std::cout << "    f : First Legal Player]\n";
}

void
print_help() {
    std::cout << "./ceramic-terminal-player [-h] [-c] [players] [-n]\n";
    std::cout << '\n';
    std::cout << "    -h : Help, shows this]\n";
    std::cout << "    -c {f,g,n} : Change color, use 'f' for full-color, 'g' for grey-scale, 'n' for none]\n";
    std::cout << '\n';
    std::cout << "    [players] list of char for each opposing player (default is 'rrr')\n";
    print_help_players();
    std::cout << '\n';
    std::cout << "    -n 2-" << TILE_TYPES << " : Change number of tile types\n";
    std::cout << '\n';
    std::cout << "In-game, enter -help form help" << std::endl;
}

bool
options(int argc, char* argv[], std::shared_ptr<TerminalPlayer>& terminal_player, std::shared_ptr<Rules>& rules) {
    int option;
    while ((option = getopt(argc, argv, ":c:")) != -1) { //get option from the getopt() method
        switch (option) {
            // Options
            case 'h':
                print_help();
                break;
            case 'c':
                switch (optarg[0]) {
                    case 'f':
                        terminal_player->set_color_type(TerminalPlayer::COLORED_TYPE::FULL);
                        break;
                    case 'g':
                        terminal_player->set_color_type(TerminalPlayer::COLORED_TYPE::GREY);
                        break;
                    case 'n':
                        terminal_player->set_color_type(TerminalPlayer::COLORED_TYPE::NONE);
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
                        throw out_of_range("");
                    }
                    rules->tile_types = value;
                } catch (const exception& e) {
                    std::cout << "Unrecognised tile types cout: " << optarg << '\n';
                    std::cout << "Use an int between 2 and " << TILE_TYPES << " (included)" << std::endl;
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
    return true;
}

int
main(int argc, char* argv[]) {
    std::shared_ptr<TerminalPlayer> terminal_player = std::make_shared<TerminalPlayer>();
    std::shared_ptr<Rules> rules = std::make_shared<Rules>(*Rules::DEFAULT);
    if (!options(argc, argv, terminal_player, rules)) {
        return 1;
    }
    // Game
    string player_chars = "rrr";
    if (optind < argc) {
        player_chars = string(argv[optind]);
    }
    rules->player_count = 1 + player_chars.size();
    Game game = Game(rules);
    game.add_player(terminal_player);
    for (char c : player_chars) {
        std::shared_ptr<Player> player;
        switch (c) {
            case 'r':
                player = std::make_shared<RandomPlayer>();
                break;
            case 'f':
                player = std::make_shared<FirstLegalPlayer>();
                break;
            default:
                std::cout << "Unrecognised player " << c << '\n';
                print_help_players();
                return 1;
        }
        game.add_player(player);
    }
    game.roll_game();
}
