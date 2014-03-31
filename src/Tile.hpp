/* Copyright 2014 <Piotr Derkowski> */

#ifndef TILE_H_
#define TILE_H_

class Tile {
public:
    enum Type {
        Empty,
        Water,
        Plains,
        Hills,
        Mountains
    };

    Tile();
    explicit Tile(Type type);

    Type getType();
    void setType(Type type);

private:
    Type type_;
};

#endif  // TILE_H_