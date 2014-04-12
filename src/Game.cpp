/* Copyright 2014 <Piotr Derkowski> */

#include <cstdlib>
#include <algorithm>
#include <string>
#include "MapDrawer.hpp"
#include "MapGenerator.hpp"
#include "Game.hpp"

Game::Game(const std::string& name, std::shared_ptr<Map> map, const MapDrawer& mapDrawer)
    : map_(map),
    mapDrawer_(mapDrawer),
    window_(
        sf::VideoMode::getFullscreenModes()[0],
        name,
        sf::Style::Fullscreen),
    mapView_(sf::FloatRect(0, 0, window_.getSize().x, window_.getSize().y))
{ }

void Game::start() {
    while (window_.isOpen()) {
        handleEvents();

        window_.clear();
        drawMap();
        window_.display();
    }
}

void Game::drawMap() {
    window_.setView(mapView_);
    window_.draw(mapDrawer_);
}

void Game::handleEvents() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
        } else if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Button::Left) {
            handleLeftClick(event);
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Key::Space) {
                handleSpacePressed();
            } else if (event.key.code == sf::Keyboard::Key::Escape) {
                handleEscapePressed();
            }
        } else if (event.type == sf::Event::MouseMoved) {
            handleMouseMoved(event);
        }
    }
}

void Game::handleLeftClick(const sf::Event& event) {
    sf::Vector2i mousePosition(event.mouseButton.x, event.mouseButton.y);

    int column = mapDrawer_.convertXCoordsToColumnNo(window_.mapPixelToCoords(mousePosition).x);
    int row = mapDrawer_.convertYCoordsToRowNo(window_.mapPixelToCoords(mousePosition).y);

    if (column != Map::OutOfBounds && row != Map::OutOfBounds)
        map_->toggleVisibility(row, column);
}

void Game::handleSpacePressed() {
    *map_ = MapGenerator::generateMap(map_->getRowsNo(), map_->getColumnsNo());
}

void Game::handleEscapePressed() {
    exit(EXIT_SUCCESS);
}

void Game::handleMouseMoved(const sf::Event& event) {
    const int scrollMarginSize = 20;

    mapView_.move(calculateHorizontalShift(event.mouseMove.x, scrollMarginSize),
        calculateVerticalShift(event.mouseMove.y, scrollMarginSize));
}

float Game::calculateHorizontalShift(float mouseXPosition, float scrollMarginSize) const {
    if (mouseXPosition < scrollMarginSize) {
        return std::max(mouseXPosition - scrollMarginSize,
            -(mapView_.getCenter().x - mapView_.getSize().x / 2));
    } else if (mouseXPosition > window_.getSize().x - scrollMarginSize) {
        return std::min(scrollMarginSize - (window_.getSize().x - mouseXPosition),
            mapDrawer_.width() - (mapView_.getCenter().x + mapView_.getSize().x / 2));
    } else {
        return 0;
    }
}

float Game::calculateVerticalShift(float mouseYPosition, float scrollMarginSize) const {
    if (mouseYPosition < scrollMarginSize) {
        return std::max(mouseYPosition - scrollMarginSize,
            -(mapView_.getCenter().y - mapView_.getSize().y / 2));
    } else if (mouseYPosition > window_.getSize().y - scrollMarginSize) {
        return std::min(scrollMarginSize - (window_.getSize().y - mouseYPosition),
            mapDrawer_.height() - (mapView_.getCenter().y + mapView_.getSize().y / 2));
    } else {
        return 0;
    }
}
