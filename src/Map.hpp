/* Copyright 2014 <Piotr Derkowski> */

#ifndef MAP_HPP_
#define MAP_HPP_

#include <vector>
#include <memory>
#include "SFML/Graphics.hpp"
#include "MapModel.hpp"
#include "MapDrawer.hpp"
#include "Tile.hpp"

class Map {
public:
    Map(int rows, int columns, std::shared_ptr<sf::RenderWindow> target, Resources& resources);
    virtual ~Map() { }

    virtual void draw() const;

    void generateMap();

    void handleClick(const sf::Event& e);
    void handleMouseWheelMoved(const sf::Event& event);
    void handleMouseMoved(const sf::Event& event);

private:
    std::shared_ptr<MapModel> model_;
    MapDrawer drawer_;
};

#endif  // MAP_HPP_
