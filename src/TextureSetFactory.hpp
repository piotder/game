/* Copyright 2014 <Piotr Derkowski> */

#ifndef TEXTURESETFACTORY_HPP_
#define TEXTURESETFACTORY_HPP_

#include "TextureSet.hpp"
#include "Tile.hpp"

class TextureSetFactory {
public:
    static TextureSet getBaseTextureSet();
    static TextureSet getBlendTextureSet();
    static TextureSet getOverlayTextureSet();
    static TextureSet getAttributeTextureSet();
};

#endif  // TEXTURESETFACTORY_HPP_
