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

void Particle::AddForce(const Vec2& force) {
    netForce += force;
}

void Particle::ClearForces() {
    netForce = Vec2(0.0, 0.0);
}

void Particle::Integrate(float dt) {
    // Find the acceleration based on the forces that are being applied
    acceleration = netForce / mass;

    // Integrate the acceleration to find the new velocity
    velocity += acceleration * dt;

    // Integrate the velocity to find the position
    position += velocity * dt;

    ClearForces();
}