#include "Particle.h"
#include <iostream>

Particle::Particle(float x, float y, float mass, int radius) {
    this->radius = radius;
    this->position = Vec2(x,y);
    this->mass = mass;
    std::cout << "Particle constructor called!" << std::endl;
}

Particle::~Particle() {
    std::cout << "Particle destructor called!" << std::endl;
}

void Particle::Integrate(float dt) {
    velocity += acceleration * dt;
    position += velocity * dt;
}