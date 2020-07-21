#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include "../global.hpp"

class Parameters {
public:
  ushort player_count;
  ushort tile_count; //tile count of each tile type
  ushort line_bonus;
  ushort column_bonus;
  ushort type_bonus;
  ushort overflow_count;
  ushort overflow_penalty;

  Parameters();
  Parameters(const Parameters& parameters);
};

#endif //PARAMETERS_HPP
