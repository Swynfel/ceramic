#include "rules.hpp"

#include <sstream>

ushort
Rules::penalty_at(ushort position) const {
    if (position >= overflow_count) {
        return 0;
    }
    return (ushort)(1 + position * (overflow_penalty - 1.0) / (overflow_count - 1) + 0.5);
}

ushort
Rules::penalty_for_floor(ushort floor) const {
    ushort penalty = 0;
    for (int i = 0; i < floor && i < overflow_count; i++) {
        penalty += penalty_at(i);
    }
    return penalty;
}

ushort
Rules::factory_count() const {
    return 1 + 2 * player_count;
}

ushort
Rules::tile_types_2() const {
    return tile_types * tile_types;
}

const std::shared_ptr<const Rules> Rules::DEFAULT = std::make_shared<Rules>();
const std::shared_ptr<const Rules> Rules::MINI = std::make_shared<Rules>(Rules{
    .player_count = 2,
    .tile_count = 15,
    .tile_types = 3,
    .factory_tiles = 3,
    .line_bonus = 3,
    .column_bonus = 2,
    .type_bonus = 5,
    .overflow_count = 4,
    .overflow_penalty = 3,
});


bool
operator==(const Rules left, const Rules right) {
    return left.player_count == right.player_count &&
           left.tile_count == right.tile_count &&
           left.tile_types == right.tile_types &&
           left.factory_tiles == right.factory_tiles &&
           left.line_bonus == right.line_bonus &&
           left.column_bonus == right.column_bonus &&
           left.type_bonus == right.type_bonus &&
           left.overflow_count == right.overflow_count &&
           left.overflow_penalty == right.overflow_penalty;
}

bool
operator!=(const Rules left, const Rules right) {
    return !(left == right);
}


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
    os << "<Ceramic Rules (" << player_count << "p)" << this << ">";
    return os.str();
}