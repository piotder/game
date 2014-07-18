/* Copyright 2014 <Piotr Derkowski> */

#ifndef SELECTION_HPP_
#define SELECTION_HPP_

#include <memory>
#include "Tile.hpp"
#include "units/Unit.hpp"
class MapModel;
class Player;

class Selection {
public:
    Selection();
    explicit Selection(std::shared_ptr<const Tile> source);

    void setSource(std::shared_ptr<const Tile> source);
    void setDestination(std::shared_ptr<const Tile> destination);
    void clear();

    bool isUnitSelected(const Player* player) const;
    bool isSourceSelected() const;
    bool isDestinationSelected() const;

    std::shared_ptr<const Tile> getSource() const;
    std::shared_ptr<const Tile> getDestination() const;

private:
    std::shared_ptr<const Tile> source_;
    std::shared_ptr<const Tile> destination_;
};

#endif  // SELECTION_HPP_
