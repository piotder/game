/* Copyright 2014 <Piotr Derkowski> */

#include <memory>
#include "SFML/Graphics.hpp"
#include "MapModel.hpp"
#include "MapDrawer.hpp"
#include "Tile.hpp"
#include "Coordinates.hpp"
#include "TextureSetFactory.hpp"
#include "Layer.hpp"
#include "MapRenderer.hpp"

MapDrawer::MapDrawer(const MapModel& model, const MapRenderer* renderer)
    : renderer_(renderer)
{
    setModel(model);
}

void MapDrawer::setModel(const MapModel& model) {
    layers_.clear();
    layers_.push_back(Layer<Tile>(TextureSetFactory::getBaseTextureSet()));
    layers_.push_back(Layer<Tile>(TextureSetFactory::getBlendTextureSet()));
    layers_.push_back(Layer<Tile>(TextureSetFactory::getOverlayTextureSet()));
    layers_.push_back(Layer<Tile>(TextureSetFactory::getAttributeTextureSet()));

    for (int r = 0; r < model.getRowsNo(); ++r) {
        for (int c = 0; c < model.getColumnsNo(); ++c) {
            auto tile = model.getTile(IntIsoPoint(c, r));
            addTileToLayers(tile);
        }
    }
}

void MapDrawer::addTileToLayers(std::shared_ptr<const Tile> tile) {
    for (auto& layer : layers_) {
        auto tilePosition = renderer_->getPosition(IntIsoPoint(tile->coords.toIsometric()));
        auto dualTilePosition = renderer_->getDualPosition(IntIsoPoint(tile->coords.toIsometric()));

        layer.add(*tile, tilePosition);
        layer.add(*tile, dualTilePosition);
    }
}

void MapDrawer::draw() const {
    MapRenderer::TargetProxy target = renderer_->getDynamicTarget();

    for (const auto& layer : layers_) {
        target.get()->draw(layer);
    }
}
