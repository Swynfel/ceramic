#ifndef RULES_HPP
#define RULES_HPP

#include "../global.hpp"

class Rules {
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

};

#endif //RULE_HPP
