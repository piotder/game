/* Copyright 2014 <Piotr Derkowski> */


#include <random>
#include "Random.hpp"


std::default_random_engine Random::generator;

void Random::initialize(unsigned seed) {
    generator.seed(seed);
}

unsigned Random::getNumber() {
    return generator();
}
