#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <iostream>
#include <limits>
#include <random>
#include <time.h>

typedef std::mt19937 rng;
static std::random_device global_random_device{};
// Adds time because std::random_device can be deterministic for some compilers
static int global_random_offset = time(NULL);

typedef std::uniform_int_distribution<ushort>::param_type ushort_range_params;
typedef std::uniform_int_distribution<ushort> ushort_range;

inline int
random_seed() {
    return global_random_device() + global_random_offset;
}

inline int
random_range(rng& randomness, ushort_range& range, ushort min = 0, ushort max = std::numeric_limits<ushort>::max()) {
    return range(randomness, ushort_range_params(min, max - 1));
}

#endif //RANDOM_HPP