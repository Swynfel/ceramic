#include "rules.hpp"

#include <sstream>

Rules::Rules()
  : player_count(4)
  , tile_count(20)
  , tile_types(TILE_TYPES)
  , factory_tiles(4)
  , line_bonus(7)
  , column_bonus(3)
  , type_bonus(15)
  , overflow_count(7)
  , overflow_penalty(3) {}

Rules::Rules(const Rules& rules)
  : player_count(rules.player_count)
  , tile_count(rules.tile_count)
  , tile_types(rules.tile_types)
  , factory_tiles(rules.factory_tiles)
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

const Rules Rules::DEFAULT = Rules();

ostream&
operator<<(ostream& os, const Rules& rules) {
    os << "player_count:     " << rules.player_count << endl;
    os << "tile_count:       " << rules.tile_count << endl;
    os << "tile_types:       " << rules.tile_types << endl;
    os << "line_bonus:       " << rules.line_bonus << endl;
    os << "column_bonus:     " << rules.column_bonus << endl;
    os << "type_bonus:       " << rules.type_bonus << endl;
    os << "overflow_count:   " << rules.overflow_count << endl;
    os << "overflow_penalty: " << rules.overflow_penalty << endl;
    return os;
}

string
Rules::str() const {
    ostringstream os;
    os << *this;
    return os.str();
}

string
Rules::repr() const {
    ostringstream os;
    os << "<Ceramic Rules (" << player_count << "p)" << this << ">" << endl;
    return os.str();
}