#ifndef WALL_HPP
#define WALL_HPP

#include <vector>

#include "global.hpp"
#include "rules/rules.hpp"
#include "tiles.hpp"

class Wall {
private:
    const std::shared_ptr<const Rules> rules;
    std::vector<Tile> placed;

    void assert_line(ushort line) const;
    void assert_column(ushort column) const;
    Tile get_tile_at_unsafe(ushort x, ushort y) const;
    void set_tile_at_unsafe(ushort x, ushort y, Tile tile);

public:
    Wall(const std::shared_ptr<const Rules>& rule);
    Wall(const Wall& wall);

    Wall& operator=(const Wall& other);
    friend bool operator==(const Wall& left, const Wall& right);
    friend bool operator!=(const Wall& left, const Wall& right);

    void clear();

    bool get_placed_at(ushort x, ushort y) const;
    Tile get_tile_at(ushort x, ushort y) const;
    Tile color_at(ushort x, ushort y) const;

    std::vector<bool> get_placed() const;
    const std::vector<Tile>& get_tiles() const;
    std::vector<std::vector<bool>> get_placed_array() const;
    std::vector<std::vector<Tile>> get_tiles_array() const;

    bool line_has_color(ushort line, Tile color) const;
    ushort line_color_x(ushort line, Tile color) const;

    ushort completed_column_count() const;
    ushort column_tile_count(ushort column) const;
    ushort completed_line_count() const;
    ushort line_tile_count(ushort line) const;
    ushort completed_type_count() const;
    ushort type_tile_count(ushort type) const;
    ushort final_score_bonus() const;

    // Scoring
    ushort score_for_placing(ushort x, ushort y);
    ushort place_at(ushort x, ushort y);
    ushort set_tile_at(ushort x, ushort y, Tile tile);
    ushort place_line_color(ushort line, Tile color);

    // Reading
    void stream_line(std::ostream& os, ushort line, bool brackets) const;
    friend std::ostream& operator<<(std::ostream& os, const Wall& wall);
    std::string str() const;
    std::string repr() const;
};

#endif //WALL_HPP
