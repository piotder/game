/* Copyright 2014 <Piotr Derkowski> */

#ifndef INTERFACE_UNITFRAME_HPP_
#define INTERFACE_UNITFRAME_HPP_

#include "SFML/Graphics.hpp"
namespace players { class Players; }
namespace units { class Unit; }
class Renderer;


namespace interface {


class UnitFrame {
public:
    explicit UnitFrame(const Renderer* renderer);

    void setPosition(const sf::Vector2f& basePosition);

    sf::Vector2f getSize() const;

    void setUnitDisplayed(const units::Unit& unit);
    void clear();

    void draw() const;

private:
    sf::Vector2f getHeight(const sf::Text& text) const;

private:
    sf::Font font_;

    unsigned characterSize_;
    sf::Vector2f lineSpacing_;
    sf::Vector2f horizontalSpacing_;
    sf::Vector2f verticalSpacing_;

    sf::Text unitName_;
    sf::Text hpLeft_;
    sf::Text movesLeft_;

    const Renderer* renderer_;
};


}


#endif  // INTERFACE_UNITFRAME_HPP_
