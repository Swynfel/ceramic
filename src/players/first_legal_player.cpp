#include "first_legal_player.hpp"

#include "game/game.hpp"
#include <limits>

std::shared_ptr<Player>
FirstLegalPlayer::copy() const {
    return std::make_shared<FirstLegalPlayer>();
}

Action
FirstLegalPlayer::play(const State& state) {
    auto rules = state.get_rules();
    for (ushort place = rules->tile_types; place != std::numeric_limits<ushort>::max(); place--) {
        for (ushort pick = 0; pick <= rules->factory_count(); pick++) {
            for (ushort color = 0; color < rules->tile_types; color++) {
                Action action = Action{ .pick = pick, .color = color, .place = place };
                if (Game::legal(action, state)) {
                    return action;
                }
            }
        }
    }
    throw std::runtime_error("No action was legal");
}


std::string
FirstLegalPlayer::player_type() const {
    return "first-legal";
}