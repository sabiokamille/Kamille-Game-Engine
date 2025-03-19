#include "Body.h"
#include <iostream>

Body::Body(float x, float y, float mass, int radius) {
    this->radius = radius;
    this->position = Vec2(x,y);
    this->mass = mass;
    if (mass != 0.0){
        this->InvMass = 1.0 / mass;
    } else {
        this->InvMass = 0.0;
    }

    std::cout << "Body constructor called!" << std::endl;
}

Body::~Body() {
    std::cout << "Body destructor called!" << std::endl;
}

void Body::AddForce(const Vec2& force) {
    netForce += force;
}

void Body::ClearForces() {
    netForce = Vec2(0.0, 0.0);
}

void Body::Integrate(float dt) {
    // Find the acceleration based on the forces that are being applied
    acceleration = netForce * InvMass;

    // Integrate the acceleration to find the new velocity
    velocity += acceleration * dt;

    // Integrate the velocity to find the position
    position += velocity * dt;

    ClearForces();
}