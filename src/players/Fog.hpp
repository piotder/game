/* Copyright 2014 <Piotr Derkowski> */

#ifndef PLAYERS_FOG_HPP_
#define PLAYERS_FOG_HPP_

#include <functional>
#include <vector>
#include "map/Tile.hpp"

namespace players {


enum class TileVisibility {
    VisibleKnown,
    UnvisibleKnown,
    Unknown
};


class Fog {
public:
    Fog(size_t rows, size_t columns);

    TileVisibility operator ()(size_t row, size_t column) const;

    size_t getRowsNo() const;
    size_t getColumnsNo() const;

    void addVisible(const std::vector<const map::Tile*>& tiles);
    void removeVisible(const std::vector<const map::Tile*>& tiles);

    void toggle();

    void clear();

private:
    TileVisibility translate(int code) const;

private:
    size_t rows_;
    size_t columns_;

    std::vector<std::vector<int>> tiles_;

    bool isFogToggledOn_;
};


}  // namespace players


namespace std {

template <>
struct hash<players::TileVisibility>
{
    std::size_t operator()(const players::TileVisibility& tileVisibility) const
    {
        static std::hash<int> typeHasher;
        return typeHasher(static_cast<int>(tileVisibility));
    }
};

}  // namespace std


#endif  // PLAYERS_FOG_HPP_
