#include "Contact.h"
#include <iostream>

void Contact::ResolvePenetration() {
    if (a->IsStatic() && b->IsStatic()) {
        return;
    }
    
    float da = depth / (a->invMass + b->invMass) * a->invMass;
    float db = depth / (a->invMass + b->invMass) * b->invMass;

    a->position -= normal * da;
    b->position += normal * db;
}

///////////////////////////////////////////////////////////////////////////////
// Resolves the collision using the impulse method
///////////////////////////////////////////////////////////////////////////////
void Contact::ResolveCollision() {
    // Apply positional correction using the projection method
    ResolvePenetration();

    // Define elasticity (coefficient of restitution E)
    float e = std::min(a->restitution, b->restitution);

    // Calculate the relative velocity between the two objects
    const Vec2 vrel = (a->velocity - b->velocity);

    // Calculate the relative velocity along the normal collision vector
    float vrelDotNormal = vrel.Dot(normal);

    // Calculate the collision impulse
    const Vec2 impulseDirection = normal;
    const float impulseMagnitude = -(1 + e) * vrelDotNormal / (a->invMass + b->invMass);
    std::cout << impulseMagnitude << std::endl;

    Vec2 jn = impulseDirection * impulseMagnitude;

    // Apply the imulse vector to both objects in opposite direction
    a->ApplyImpulse(jn);
    b->ApplyImpulse(-jn);

}