#ifndef BODY_H
#define BODY_H


#include "Vec2.h"
#include "Shape.h"

// TODO:
struct Body {
    int radius;

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    float angle;

    Vec2 netForce;

    float mass;
    float InvMass;

    // A pointer to the geometry shape of the rigid body (circle, box, polygon, etc)
    Shape* shape = NULL;

    Body(const Shape& shape, float x, float y, float mass, int radius);
    ~Body();

    void AddForce(const Vec2& force);
    void ClearForces();

    void Integrate(float dt);
};

#endif