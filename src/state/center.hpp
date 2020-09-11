#ifndef CENTER_HPP
#define CENTER_HPP

#include <string>

#include "global.hpp"
#include "tiles.hpp"

class Center : public Tiles {
public:
    bool first_token;
    // Constructor
    Center();

    void set_tiles(const Tiles tiles);

    // Reading
    friend ostream& operator<<(ostream& os, const Center& center);
    string str() const;
    string repr() const;
};

#endif //CENTER_HPP
