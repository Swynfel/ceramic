#ifndef RULES_HPP
#define RULES_HPP

#include <memory>
#include <string>

#include "../global.hpp"

using namespace std;

class Rules {

public:
    ushort player_count;
    ushort tile_count; //tile count of each tile type
    ushort tile_types;
    ushort factory_tiles;
    ushort line_bonus;
    ushort column_bonus;
    ushort type_bonus;
    ushort overflow_count;
    ushort overflow_penalty;

    Rules();
    Rules(const Rules& rules);

    const ushort penalty_at(ushort position) const;
    const ushort penalty_for_floor(ushort floor) const;
    const ushort factories() const;

    static const Rules DEFAULT;

    // Reading
    friend ostream& operator<<(ostream& os, const Rules& rules);
    string str() const;
    string repr() const;
};

#endif //RULE_HPP
