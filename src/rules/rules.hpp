#ifndef RULES_HPP
#define RULES_HPP

#include <memory>
#include <string>

#include "global.hpp"

struct Rules {
    ushort player_count = 4;
    ushort tile_count = 20; //tile count of each tile type
    ushort tile_types = 5;
    ushort factory_tiles = 4;
    ushort line_bonus = 7;
    ushort column_bonus = 3;
    ushort type_bonus = 15;
    ushort overflow_count = 7;
    ushort overflow_penalty = 3;

    friend bool operator==(const Rules left, const Rules right);
    friend bool operator!=(const Rules left, const Rules right);

    ushort penalty_at(ushort position) const;
    ushort penalty_for_floor(ushort floor) const;
    ushort factory_count() const;
    ushort tile_types_2() const;

    static const std::shared_ptr<const Rules> DEFAULT;
    static const std::shared_ptr<const Rules> MINI;

    // Reading
    friend std::ostream& operator<<(std::ostream& os, const Rules& rules);
    std::string str() const;
    std::string repr() const;
};

#endif //RULE_HPP
