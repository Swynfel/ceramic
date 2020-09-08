#include "state.hpp"

State::State(const std::shared_ptr<Rules>& rules)
  : rules(rules)
  , factories(rules->factories(), Factory(rules->player_count))
  , panels(rules->player_count, Panel(rules))
  , bag()
  , bin() {}

void
State::start() {
    bag = Tiles(vector<ushort>(rules->tile_types, rules->tile_count));
    for(Factory& factory: factories) {
        factory.tiles = Tiles::ZERO;
    }
    for(Panel& panel: panels) {
        panel.clear();
    }
}

void
State::start_round() {

}