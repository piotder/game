/* Copyright 2014 <Piotr Derkowski> */

#include <algorithm>
#include <stdexcept>
#include "Tile.hpp"
#include "Coordinates.hpp"
#include "MapModel.hpp"
#include "TileEnums.hpp"

using namespace tileenums;


namespace map {


Tile::Tile(const IntRotPoint& coords, Type type, MapModel* model)
    : coords(coords), type(type), model_(model)
{ }

void Tile::setModel(MapModel* model) {
    model_ = model;
}

bool Tile::hasNeighbor(Direction direction) const {
    auto neighborCoords = getNeighborCoords(direction);
    return model_->isInBounds(IntIsoPoint(neighborCoords.toIsometric()));
}

const Tile& Tile::getNeighbor(Direction direction) const {
    if (isValid()) {
        if (hasNeighbor(direction)) {
            return model_->getTile(IntIsoPoint(getNeighborCoords(direction).toIsometric()));
        } else {
            throw std::logic_error("Requested neighbor doesn't exist.");
        }
    } else {
        throw std::logic_error("Trying to find neighbors of invalid tile.");
    }
}

std::vector<const Tile*> Tile::getNeighbors() const {
    std::vector<const Tile*> neighbors;

    if (hasNeighbor(Direction::Top))
        neighbors.push_back(&getNeighbor(Direction::Top));
    if (hasNeighbor(Direction::TopRight))
        neighbors.push_back(&getNeighbor(Direction::TopRight));
    if (hasNeighbor(Direction::Right))
        neighbors.push_back(&getNeighbor(Direction::Right));
    if (hasNeighbor(Direction::BottomRight))
        neighbors.push_back(&getNeighbor(Direction::BottomRight));
    if (hasNeighbor(Direction::Bottom))
        neighbors.push_back(&getNeighbor(Direction::Bottom));
    if (hasNeighbor(Direction::BottomLeft))
        neighbors.push_back(&getNeighbor(Direction::BottomLeft));
    if (hasNeighbor(Direction::Left))
        neighbors.push_back(&getNeighbor(Direction::Left));
    if (hasNeighbor(Direction::TopLeft))
        neighbors.push_back(&getNeighbor(Direction::TopLeft));

    return neighbors;
}

std::vector<const Tile*> Tile::getAdjacentNeighbors() const {
    std::vector<const Tile*> neighbors;

    if (hasNeighbor(Direction::Top))
        neighbors.push_back(&getNeighbor(Direction::Top));
    if (hasNeighbor(Direction::Right))
        neighbors.push_back(&getNeighbor(Direction::Right));
    if (hasNeighbor(Direction::Bottom))
        neighbors.push_back(&getNeighbor(Direction::Bottom));
    if (hasNeighbor(Direction::Left))
        neighbors.push_back(&getNeighbor(Direction::Left));

    return neighbors;
}

std::vector<Tile*> Tile::getAdjacentNeighbors() {
    std::vector<const Tile*> neighbors = static_cast<const Tile*>(this)->getAdjacentNeighbors();

    std::vector<Tile*> res(neighbors.size());
    for (size_t i = 0; i < neighbors.size(); ++i) {
        res[i] = const_cast<Tile*>(neighbors[i]);
    }

    return res;
}

Direction Tile::getDirection(const Tile& neighbor) const {
    if (hasNeighbor(Direction::Top) && neighbor == getNeighbor(Direction::Top)) {
        return Direction::Top;
    } else if (hasNeighbor(Direction::TopRight) && neighbor == getNeighbor(Direction::TopRight)) {
        return Direction::TopRight;
    } else if (hasNeighbor(Direction::Right) && neighbor == getNeighbor(Direction::Right)) {
        return Direction::Right;
    } else if (hasNeighbor(Direction::BottomRight) && neighbor == getNeighbor(Direction::BottomRight)) {
        return Direction::BottomRight;
    } else if (hasNeighbor(Direction::Bottom) && neighbor == getNeighbor(Direction::Bottom)) {
        return Direction::Bottom;
    } else if (hasNeighbor(Direction::BottomLeft) && neighbor == getNeighbor(Direction::BottomLeft)) {
        return Direction::BottomLeft;
    } else if (hasNeighbor(Direction::Left) && neighbor == getNeighbor(Direction::Left)) {
        return Direction::Left;
    } else if (hasNeighbor(Direction::TopLeft) && neighbor == getNeighbor(Direction::TopLeft)) {
        return Direction::TopLeft;
    } else {
        throw std::invalid_argument("Tile " + toString(neighbor.coords) + " is not a neighbor of "
            + toString(coords) + ".");
    }
}

bool Tile::isValid() const {
    return model_ != nullptr;
}

IntRotPoint Tile::getNeighborCoords(Direction direction) const {
    switch (direction) {
    case Direction::Top:
        return IntRotPoint(coords.x, coords.y - 1);
    case Direction::TopRight:
        return IntRotPoint(coords.x + 1, coords.y - 1);
    case Direction::Right:
        return IntRotPoint(coords.x + 1, coords.y);
    case Direction::BottomRight:
        return IntRotPoint(coords.x + 1, coords.y + 1);
    case Direction::Bottom:
        return IntRotPoint(coords.x, coords.y + 1);
    case Direction::BottomLeft:
        return IntRotPoint(coords.x - 1, coords.y + 1);
    case Direction::Left:
        return IntRotPoint(coords.x - 1, coords.y);
    case Direction::TopLeft:
        return IntRotPoint(coords.x - 1, coords.y - 1);
    default:
        throw std::logic_error("Unrecognized direction.");
    }
}

std::vector<const Tile*> Tile::getTilesInRadius(int radius) const {
    std::vector<const Tile*> res;

    for (int x = -radius; x <= radius; ++x) {
        for (int y = -radius; y <= radius; ++y) {
            IntRotPoint p(coords.x + x, coords.y + y);
            IntIsoPoint pIso(p.toIsometric());

            if (model_->isInBounds(pIso))
                res.push_back(&(model_->getTile(pIso)));
        }
    }

    return res;
}



bool operator == (const Tile& lhs, const Tile& rhs) {
    return lhs.coords == rhs.coords && lhs.type == rhs.type && lhs.model_ == rhs.model_;
}

bool operator != (const Tile& lhs, const Tile& rhs) {
    return !(lhs == rhs);
}

bool operator < (const Tile& lhs, const Tile& rhs) {
    return lhs.coords < rhs.coords;
}


}  // namespace map
