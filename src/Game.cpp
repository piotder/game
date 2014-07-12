/* Copyright 2014 <Piotr Derkowski> */

#include <cstdlib>
#include <memory>
#include "Map.hpp"
#include "Menu.hpp"
#include "Game.hpp"
#include "TileEnums.hpp"

Game::Game(int rows, int columns)
        : window_(std::make_shared<sf::RenderWindow>(
            sf::VideoMode::getFullscreenModes()[0],
            "",
            sf::Style::Fullscreen)),
        map_(rows, columns, window_),
        menu_(window_)
{
    menu_.addItem("Return", [this] () { toggleMenu(); });
    menu_.addItem("New game", [this] () { restart(); });
    menu_.addItem("Quit game", [this] () { quit(); });
}

void Game::start() {
    while (window_->isOpen()) {
        handleEvents();

        window_->clear();
        map_.draw();
        if (menu_.isVisible()) {
            menu_.draw();
        }
        window_->display();
    }
}

void Game::restart() {
    map_.generateMap();
}

void Game::quit() {
    exit(EXIT_SUCCESS);
}

void Game::toggleMenu() {
    menu_.toggleVisibility();
}

void Game::handleEvents() {
    sf::Event event;
    while (window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_->close();
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Button::Left) {
                handleLeftClick(event);
            } else if (event.mouseButton.button == sf::Mouse::Button::Right) {
                handleRightClick(event);
            }
        } else if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
            case sf::Keyboard::Key::Space:
                restart();
                break;
            case sf::Keyboard::Key::Escape:
                quit();
                break;
            case sf::Keyboard::Key::M:
                toggleMenu();
                break;
            default:
                break;
            }
        } else if (event.type == sf::Event::MouseWheelMoved) {
            handleMouseWheelMoved(event);
        } else if (event.type == sf::Event::MouseMoved) {
            handleMouseMoved(event);
        }
    }
}

void Game::handleLeftClick(const sf::Event& event) {
    if (menu_.isVisible()) {
        menu_.handleLeftClick(event);
    } else {
        map_.handleLeftClick(event);
    }
}

void Game::handleRightClick(const sf::Event& event) {
    if (!menu_.isVisible()) {
        map_.handleRightClick(event);
    }
}

void Game::handleMouseWheelMoved(const sf::Event& event) {
    if (!menu_.isVisible()) {
        map_.handleMouseWheelMoved(event);
    }
}

void Game::handleMouseMoved(const sf::Event& event) {
    if (menu_.isVisible()) {
        menu_.handleMouseMoved(event);
    } else {
        map_.handleMouseMoved(event);
    }
}
