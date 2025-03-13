#ifndef PARTICLE_H
#define PARTICLE_H
#include "Vec2.h"

// TODO:
struct Particle {
    int radius;

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    Vec2 netForce;

    float mass;
    float InvMass;

    Particle(float x, float y, float mass, int radius);
    ~Particle();

    void AddForce(const Vec2& force);
    void ClearForces();

    void Integrate(float dt);
};

#endif