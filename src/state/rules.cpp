#include "rules.hpp"

Rules::Rules()
  : player_count(4)
  , tile_count(20)
  , tile_types(TILE_TYPES)
  , line_bonus(7)
  , column_bonus(3)
  , type_bonus(15)
  , overflow_count(7)
  , overflow_penalty(3) {}

Rules::Rules(const Rules& rules)
  : player_count(rules.player_count)
  , tile_count(rules.tile_count)
  , tile_types(rules.tile_types)
  , line_bonus(rules.line_bonus)
  , column_bonus(rules.column_bonus)
  , type_bonus(rules.type_bonus)
  , overflow_count(rules.overflow_count)
  , overflow_penalty(rules.overflow_penalty) {}

const ushort
Rules::penalty_at(ushort position) const {
    if (position >= overflow_count) {
        return 0;
    }
    return (ushort)(1 + position * (overflow_penalty - 1.0) / (overflow_count - 1) + 0.5);
}

const ushort
Rules::penalty_for_floor(ushort floor) const {
    ushort penalty = 0;
    for (int i = 0; i < floor && i < overflow_count; i++) {
        penalty += penalty_at(i);
    }
    return penalty;
}

const ushort
Rules::factories() const {
    return 1 + 2 * player_count;
}

// Lazy shared_default_rules initialization
std::shared_ptr<Rules> Rules::shared_default_rules = nullptr;

std::shared_ptr<Rules>
Rules::get_default() {
    if (shared_default_rules == nullptr) {
        shared_default_rules = std::make_shared<Rules>();
    }
    return shared_default_rules;
}
