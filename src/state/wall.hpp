#ifndef WALL_HPP
#define WALL_HPP

#include <vector>

#include "global.hpp"
#include "rules/rules.hpp"
#include "tiles.hpp"

class Wall {
private:
    const std::shared_ptr<Rules> rules;
    vector<Tile> placed;

    void assert_line(ushort line) const;
    void assert_column(ushort column) const;
    Tile get_tile_at_unsafe(ushort x, ushort y) const;
    void set_tile_at_unsafe(ushort x, ushort y, Tile tile);

public:
    Wall(const std::shared_ptr<Rules>& rule);
    Wall(const Wall& wall);

    void clear();

    bool get_placed_at(ushort x, ushort y) const;
    Tile get_tile_at(ushort x, ushort y) const;
    Tile color_at(ushort x, ushort y) const;

    vector<bool> get_placed() const;
    const vector<Tile>& get_tiles() const;
    vector<vector<bool>> get_placed_array() const;
    vector<vector<Tile>> get_tiles_array() const;

    bool line_has_color(ushort line, Tile color) const;
    ushort line_color_x(ushort line, Tile color) const;

    // Scoring
    ushort score_for_placing(ushort x, ushort y);
    ushort place_at(ushort x, ushort y);
    ushort set_tile_at(ushort x, ushort y, Tile tile);
    ushort place_line_color(ushort line, Tile color);

    // Reading
    void stream_line(ostream& os, ushort line, bool brackets) const;
    friend ostream& operator<<(ostream& os, const Wall& wall);
    string str() const;
    string repr() const;
};

#endif //WALL_HPP
