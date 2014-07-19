/* Copyright 2014 <Piotr Derkowski> */

#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>
#include "Utils.hpp"
#include "Coordinates.hpp"
#include "SFML/Graphics.hpp"
#include "MapRenderer.hpp"

MapRenderer::MapRenderer(int rows, int columns, int tileWidth, int tileHeight,
    std::shared_ptr<sf::RenderTarget> target)
        : rows_(rows),
        columns_(columns),
        tileWidth_(tileWidth),
        tileHeight_(tileHeight),
        target_(target),
        mapView_(sf::FloatRect(0, 0, target->getSize().x, target->getSize().y))
{ }

sf::Vector2f MapRenderer::getPosition(const IntIsoPoint& coords) const {
    CartPoint cartCoords = coords.toCartesian();
    return sf::Vector2f(utils::positiveModulo(cartCoords.x * tileWidth_ / 2, 2 * getMapWidth()),
        cartCoords.y * tileHeight_ / 2);
}

sf::Vector2f MapRenderer::getDualPosition(const IntIsoPoint& coords) const {
    sf::Vector2f primaryPosition = getPosition(coords);
    return sf::Vector2f(utils::positiveModulo(primaryPosition.x + getMapWidth(), 2 * getMapWidth()),
        primaryPosition.y);
}

unsigned MapRenderer::getMapWidth() const {
    return columns_ * tileWidth_;
}

unsigned MapRenderer::getMapHeight() const {
    return (rows_ - 1) * tileHeight_ / 2;
}

sf::Vector2u MapRenderer::getSize() const {
    return target_->getSize();
}

std::shared_ptr<sf::RenderTarget> MapRenderer::getFixedView() const {
    target_->setView(target_->getDefaultView());
    return target_;
}

std::shared_ptr<sf::RenderTarget> MapRenderer::getDynamicView() const {
    target_->setView(mapView_);
    return target_;
}

IntIsoPoint MapRenderer::getMapCoords(const sf::Vector2i& position) const {
    const int halfWidth = tileWidth_ / 2;
    const int halfHeight = tileHeight_ / 2;

    const sf::Vector2f coords = target_->mapPixelToCoords(position);
    const sf::Vector2i intCoords(lround(coords.x), lround(coords.y));
    const sf::Vector2i corner(intCoords.x - intCoords.x % halfWidth,
        intCoords.y - intCoords.y % halfHeight);
    const sf::Vector2i scaledCorner(corner.x / halfWidth, corner.y / halfHeight);

    sf::Vector2i firstCenter, otherCenter;
    if ((scaledCorner.x + scaledCorner.y) % 2 == 0) {
        firstCenter = corner;
        otherCenter = sf::Vector2i(corner.x + halfWidth, corner.y + halfHeight);
    } else {
        firstCenter = sf::Vector2i(corner.x + halfWidth, corner.y);
        otherCenter = sf::Vector2i(corner.x, corner.y + halfHeight);
    }

    const double distanceToFirst = sqrt(pow(coords.x - firstCenter.x, 2)
        + pow(coords.y - firstCenter.y, 2));
    const double distanceToOther = sqrt(pow(otherCenter.x - coords.x, 2)
        + pow(otherCenter.y - coords.y, 2));

    const sf::Vector2i closest = (distanceToFirst < distanceToOther)? firstCenter : otherCenter;
    const IntCartPoint scaledClosest = IntCartPoint(closest.x / halfWidth, closest.y / halfHeight);
    const IntIsoPoint isometric(scaledClosest.toIsometric());

    return isometric;
}

void MapRenderer::scrollView(int x, int y) {
    mapView_.move(boundShift(x, y));
    target_->setView(mapView_);
}

sf::Vector2f MapRenderer::boundShift(int x, int y) const {
    sf::Vector2f safeShift;

    sf::IntRect bounds(0, 0, getMapWidth(), getMapHeight());
    sf::IntRect displayed(mapView_.getCenter().x - mapView_.getSize().x / 2,
        mapView_.getCenter().y - mapView_.getSize().y / 2,
        mapView_.getSize().x, mapView_.getSize().y);

    int maxTopShiftPossible = bounds.top - displayed.top;
    int maxBottomShiftPossible = bounds.top + bounds.height - displayed.top - displayed.height;

    safeShift.x
        = static_cast<int>((displayed.left + x + getMapWidth()) % getMapWidth())
            - displayed.left;
    safeShift.y = (y < 0)? std::min(std::max(y, maxTopShiftPossible), maxBottomShiftPossible)
        : std::max(std::min(y, maxBottomShiftPossible), maxTopShiftPossible);

    return safeShift;
}

void MapRenderer::zoomView(int delta, const sf::Vector2i& mousePosition) {
    target_->setView(mapView_);
    const float zoomFactor = (delta > 0)? 4.0 / 5 : 5.0 / 4;

    if (canZoom(zoomFactor)) {
        target_->setView(mapView_);
        sf::Vector2f oldCoords = target_->mapPixelToCoords(mousePosition);
        mapView_.zoom(zoomFactor);
        target_->setView(mapView_);

        sf::Vector2f newCoords = target_->mapPixelToCoords(mousePosition);
        scrollView(oldCoords.x - newCoords.x, oldCoords.y - newCoords.y);
    }
}

bool MapRenderer::canZoom(float zoomFactor) const {
    return mapView_.getSize().x * zoomFactor < getMapWidth()
        && mapView_.getSize().y * zoomFactor < getMapHeight();
}

sf::FloatRect MapRenderer::getDisplayedRectangle() const {
    target_->setView(mapView_);
    sf::Vector2i leftTop(0, 0);
    sf::Vector2i rightBottom(target_->getSize().x - 1, target_->getSize().y - 1);

    sf::Vector2f leftTopCoords = target_->mapPixelToCoords(leftTop);
    sf::Vector2f rightBottomCoords = target_->mapPixelToCoords(rightBottom);

    return sf::FloatRect(
        leftTopCoords.x / getMapWidth(),
        leftTopCoords.y / getMapHeight(),
        (rightBottomCoords.x - leftTopCoords.x) / getMapWidth(),
        (rightBottomCoords.y - leftTopCoords.y) / getMapHeight());
}