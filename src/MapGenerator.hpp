/* Copyright 2014 <Piotr Derkowski> */

#ifndef MAPGENERATOR_HPP_
#define MAPGENERATOR_HPP_

#include <vector>
#include "Tile.hpp"
#include "Map.hpp"

class MapGenerator {
public:
    static Map generateMap(int rows, int columns);

private:
    static void assignTileTypes(Map& map);
    static std::vector<std::vector<float>> generateHeights(int rows, int columns);
    static std::vector<std::vector<float>> getInitialHeights(int rows, int columns);
    static void relaxHeights(std::vector<std::vector<float>>& heights);
    static float calculateDelta(float lhs, float rhs);
    static Tile::Type convertHeightToTileType(float height);
};

#endif  // MAPGENERATOR_HPP_
