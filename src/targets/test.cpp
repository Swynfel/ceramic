#include <iostream>

#include "game/game.hpp"
#include "game/player.hpp"
#include "players/random_player.hpp"

int
main() {
    std::shared_ptr<Rules> rules(Rules::DEFAULT);
    std::cout << "Rules: " << *rules << std::endl;
    Game game = Game(rules);
    std::shared_ptr<Player> random_player = std::make_shared<RandomPlayer>();
    for (ushort p = 0; p < rules->player_count; p++) {
        game.add_player(random_player);
    }
    std::cout << "Starting game" << std::endl;
    game.roll_game();
    const State& state = game.get_state();
    std::cout << "Winner: " << state.winning_player() << std::endl;
    std::cout << "Scores:";
    for (ushort p = 0U; p < rules->player_count; p++) {
        std::cout << " " << state.get_panel(p).get_score();
    }
    std::cout << std::endl;
}
