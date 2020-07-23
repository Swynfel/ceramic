#ifndef STATE_HPP
#define STATE_HPP

#include <boost/random/mersenne_twister.hpp>

#include "tiles.hpp"
#include "factory.hpp"

typedef boost::random::mt11213b rng;

class State {
private:
  rng randomness;
public:
  State();

};

#endif //STATE_HPP
