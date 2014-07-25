/* Copyright 2014 <Piotr Derkowski> */

#ifndef TILE_HPP_
#define TILE_HPP_

#include "Attributes.hpp"
#include "Coordinates.hpp"
#include "TileEnums.hpp"
class MapModel;

using namespace tileenums;

class Tile {
public:
    explicit Tile(const IntRotPoint& coords = IntRotPoint(-1, -1), Type type = Type::Empty,
        MapModel* model = nullptr);

    void setModel(MapModel* model);

    bool hasNeighbor(Direction direction) const;
    const Tile& getNeighbor(Direction direction) const;
    std::vector<const Tile*> getNeighbors() const;

    std::vector<const Tile*> getAdjacentNeighbors() const;
    std::vector<Tile*> getAdjacentNeighbors();

    std::vector<const Tile*> getTilesInRadius(int radius) const;

    Direction getDirection(const Tile& neighbor) const;

    bool isValid() const;

private:
    friend bool operator == (const Tile& lhs, const Tile& rhs);
    friend bool operator != (const Tile& lhs, const Tile& rhs);
    friend bool operator < (const Tile& lhs, const Tile& rhs);
    friend class std::hash<Tile>;

    IntRotPoint getNeighborCoords(Direction direction) const;

public:
    IntRotPoint coords;
    Type type;
    Attributes attributes;

private:
    MapModel* model_;
};

bool operator == (const Tile& lhs, const Tile& rhs);
bool operator != (const Tile& lhs, const Tile& rhs);
bool operator < (const Tile& lhs, const Tile& rhs);


namespace std {


template <>
struct hash<Tile>
{
    std::size_t operator()(const Tile& tile) const
    {
        std::size_t seed = 0;

        static std::hash<IntRotPoint> coordsHasher;
        static std::hash<int> typeHasher;
        static std::hash<MapModel*> modelHasher;

        boost::hash_combine(seed, coordsHasher(tile.coords));
        boost::hash_combine(seed, typeHasher(static_cast<int>(tile.type)));
        boost::hash_combine(seed, modelHasher(tile.model_));

        return seed;
    }
};


}  // namespace std

#endif  // TILE_HPP_
