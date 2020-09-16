#include <iostream>

#include "game/game.hpp"
#include "game/player.hpp"
#include "players/random_player.hpp"
#include "players/terminal_player.hpp"

int
main() {
    std::shared_ptr<Rules> rules(Rules::DEFAULT);
    Game game = Game(rules);
    std::shared_ptr<Player> random_player = std::make_shared<RandomPlayer>();
    std::shared_ptr<TerminalPlayer> terminal_player = std::make_shared<TerminalPlayer>();
    game.add_player(terminal_player);
    for (ushort p = 1; p < rules->player_count; p++) {
        game.add_player(random_player);
    }
    std::cout << "Starting game" << std::endl;
    game.roll_game();
    const State& state = game.get_state();
    std::cout << state << std::endl;
    std::cout << "Winner: " << state.winning_player() << std::endl;
    std::cout << "Scores:";
    for (ushort p = 0U; p < rules->player_count; p++) {
        std::cout << " " << state.get_panel(p).get_score();
    }
    std::cout << std::endl;
}
