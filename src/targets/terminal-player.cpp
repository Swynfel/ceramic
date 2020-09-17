#include <iostream>

#include "game/game.hpp"
#include "game/player.hpp"
#include "players/random_player.hpp"
#include "players/terminal_player.hpp"

int
main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cout << "Can only pass one argument at most" << std::endl;
    }
    std::shared_ptr<TerminalPlayer> terminal_player = std::make_shared<TerminalPlayer>();
    if (argc == 2) {
        switch (argv[1][0]) {
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
                std::cout << "Unrecognised argument: " << argv[1] << std::endl;
                std::cout << "Use 'f' for full-color, 'g' for grey-scale, 'n' for none" << std::endl;
                return 1;
        }
    }
    // Game
    std::shared_ptr<Rules> rules = std::make_shared<Rules>(*Rules::DEFAULT);
    rules->player_count = 3;
    std::cout << "current: " << rules->player_count << std::endl;
    std::cout << "default: " << Rules::DEFAULT->player_count << std::endl;
    Game game = Game(rules);
    std::shared_ptr<Player> random_player = std::make_shared<RandomPlayer>();
    game.add_player(terminal_player);
    for (ushort p = 1; p < rules->player_count; p++) {
        game.add_player(random_player);
    }
    game.roll_game();
}
