#ifndef RULES_HPP
#define RULES_HPP

#include <memory>

#include "../global.hpp"

class Rules {
private:
    static std::shared_ptr<Rules> shared_default_rules;

public:
    ushort player_count;
    ushort tile_count; //tile count of each tile type
    ushort tile_types;
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

    static std::shared_ptr<Rules> get_default();
};

#endif //RULE_HPP
