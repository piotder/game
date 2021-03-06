/* Copyright 2014 <Piotr Derkowski> */

#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "Unit.hpp"
#include "map/Tile.hpp"
#include "Coordinates.hpp"
#include "TileEnums.hpp"
#include "map/MapModel.hpp"
#include "players/Player.hpp"

namespace units {


Unit::Unit(const IntRotPoint& coords,
    const UnitProperties& properties,
    const map::MapModel* model,
    const players::Player* owner)
        : coords_(coords), properties_(properties), model_(model), owner_(owner)
{ }

void Unit::resetMoves() {
    properties_.movesLeft = properties_.baseMoves;
}

void Unit::setMovesLeft(int movesLeft) {
    properties_.movesLeft = std::max(movesLeft, 0);
}

void Unit::setHpLeft(int hpLeft) {
    properties_.hpLeft = std::max(hpLeft, 0);
}

std::string Unit::getName() const {
    return properties_.name;
}

Type Unit::getType() const {
    return properties_.type;
}

map::Tile Unit::getPosition() const {
    if (model_ != nullptr)
        return model_->getTile(IntIsoPoint(coords_.toIsometric()));
    else
        throw std::runtime_error("No model set in unit.");
}

IntRotPoint Unit::getCoords() const {
    return coords_;
}

const players::Player* Unit::getOwner() const {
    return owner_;
}

int Unit::getMovesLeft() const {
    return properties_.movesLeft;
}

int Unit::getHpLeft() const {
    return properties_.hpLeft;
}

int Unit::getBaseHp() const {
    return properties_.baseHp;
}

int Unit::getBaseMoves() const {
    return properties_.baseMoves;
}

bool Unit::canMoveTo(tileenums::Direction direction) const {
    return getPosition().hasNeighbor(direction);
}

void Unit::moveTo(tileenums::Direction direction) {
    coords_ = getPosition().getNeighbor(direction).coords;
}

bool operator == (const Unit& lhs, const Unit& rhs) {
    return (lhs.coords_ == rhs.coords_) && (lhs.properties_ == rhs.properties_)
        && (lhs.model_ == rhs.model_) && (lhs.owner_ == rhs.owner_);
}


}  // namespace units

