#ifndef FORCE_H
#define FORCE_H

#include "./Particle.h"
#include "./Vec2.h"

struct Force {
    public:
        static Vec2 GenerateDragForce(const Particle& particle, float k);
        static Vec2 GenerateFrictionForce(const Particle& particle, float k);
};
#endif