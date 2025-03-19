#ifndef FORCE_H
#define FORCE_H

#include "./Body.h"
#include "./Vec2.h"

struct Force {
    public:
        static Vec2 GenerateDragForce(const Body& body, float k);
        static Vec2 GenerateFrictionForce(const Body& body, float k);
        static Vec2 GenerateAttractionForce(const Body& a, const Body& b, float G);
        static Vec2 GenerateSpringForce(const Body& body, Vec2 anchor, float restLength, float k);
};
#endif