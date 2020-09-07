#include "state.hpp"

State::State(const std::shared_ptr<Rules>& rules)
  : rules(rules)
  , factories(rules->factories(), Factory(rules->player_count))
  , panels(rules->player_count, Panel(rules)) {}
