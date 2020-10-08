#ifndef CENTER_HPP
#define CENTER_HPP

#include <string>

#include "global.hpp"
#include "tiles.hpp"

class Center {
public:
    bool first_token;
    Tiles tiles;

    friend bool operator==(const Center& left, const Center& right);
    friend bool operator!=(const Center& left, const Center& right);

    // Constructor
    Center();
    Center(Tiles tiles, bool first_token);

    // Reading
    friend std::ostream& operator<<(std::ostream& os, const Center& center);
    std::string str() const;
    std::string repr() const;
};

#endif //CENTER_HPP
