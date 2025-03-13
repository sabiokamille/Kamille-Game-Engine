#include "Force.h"
#include <iostream>
using namespace std;

Vec2 Force::GenerateDragForce(const Particle& particle, float k) {
    Vec2 dragForce = Vec2(0,0);

    if(particle.velocity.MagnitudeSquared() > 0) {
        // Calculate the drag direction (inverse of velocity unit vector);
        Vec2 dragDirection = particle.velocity.UnitVector() * -1.0;

        // Calculate the drag magnitude, k * ||v||^2
        float dragMagnitude = k * particle.velocity.MagnitudeSquared();

        // Generate the final drag force with direction and magnitude
        dragForce = dragDirection * dragMagnitude;
    }
    return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Particle& particle, float k) {
    Vec2 frictionForce = Vec2(0,0);

    // Calculate the friction direction (inverse of the velocity unit vector)
    Vec2 frictionDirection = particle.velocity.UnitVector() * -1.0;

    // Calculate the friction force
    frictionForce = frictionDirection * k;

    return frictionForce;
}

Vec2 Force::GenerateAttractionForce(const Particle& a, const Particle& b, float G) {
    Vec2 distance = (b.position - a.position);

    // std::cout << distance.x << " " << distance.y << endl;
    float distanceSquared = distance.MagnitudeSquared();

// Clamp the value of the distance to make for some interesting and visible physics affects
    if (distanceSquared > 50){
        distanceSquared = 50;
    } else if (distanceSquared < 5){
        distanceSquared = 5;
    }

    Vec2 attaractionDirection = distance.UnitVector();
    float attractionMagnitude = G * a.mass * b.mass / distanceSquared;

    Vec2 attractionForce = attaractionDirection * attractionMagnitude;

    return attractionForce;
}

Vec2 Force::GenerateSpringForce(const Particle& particle, Vec2 anchor, float restLength, float k){
    Vec2 springForce;
    
    //Find the distance between the anchor and the object
    Vec2 d = particle.position - anchor;

    //Find the displacement of the object from the rest length of the spring
    float displacement = d.Magnitude() - restLength;

    //Calculate the direction and magnitude of the spring force if there is one
    if (displacement == 0.0F) {
        springForce = Vec2(0,0);
    } else {
        Vec2 direction = d.UnitVector();
        float magnitude = -k * displacement;
        springForce = direction * magnitude; 
    }
    
    return springForce;
    }