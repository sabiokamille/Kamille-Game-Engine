#ifndef BODY_H
#define BODY_H


#include "Vec2.h"
#include "Shape.h"

// TODO:
struct Body {
    // Collision detection
    bool isColliding;

    // Linear motion
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    // Angular motion
    float rotation;
    float angularVelocity;
    float angularAcceleration;

    // Forces and torque
    Vec2 sumForces;
    float sumTorque;

    // Mass and Moment of Inertia
    float mass;
    float invMass;
    float I;
    float invI;

    // Coefficient of restitution
    float restitution;

    // Impulse
    void ApplyImpulse(const Vec2& jn);

    // A pointer to the geometry/shape of the rigid body (circle, box, polygon, etc)
    Shape* shape = nullptr;

    Body(const Shape& shape, float x, float y, float mass);
    ~Body();

    bool IsStatic() const;

    void AddForce(const Vec2& force);
    void AddTorque(float torque);
    void ClearForces();
    void ClearTorque();

    void IntegrateLinear(float dt);
    void IntegrateAngular(float dt);

    void Update(float dt);
};

#endif