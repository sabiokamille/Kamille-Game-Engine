#ifndef CONTACT_H
#define CONTACT_H

#include "Vec2.h"
#include "Body.h"

struct Contact {
    Body* a;
    Body* b;

    Vec2 pa;
    Vec2 pb;

    Vec2 normal;
    float depth;

    Contact() = default;
    ~Contact() = default;

    void ResolvePenetration();
    void ResolveCollision();
};

#endif