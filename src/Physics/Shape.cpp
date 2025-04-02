#include "Shape.h"
#include <iostream>

CircleShape::CircleShape(float radius) {
    this->radius =radius;
    std::cout << "CircleShape constructor called" << std::endl;
};

CircleShape::~CircleShape() {
    std::cout << "CircleShape destructor called" << std::endl;
};

ShapeType CircleShape::GetType() const {
    return CIRCLE;
};

Shape* CircleShape::Clone() const {
    return new CircleShape(radius);
};

float CircleShape::GetMomentOfInertia() const{
    // For solid circles the moment of inertia is 1/2 * 2^2
    // But this still needs to be multiplied by the mass of the RB
    return 0.5 * (radius * radius);
};

PolygonShape::PolygonShape(const std::vector<Vec2> vertices) {
    // TODO: implment it
};

PolygonShape::~PolygonShape() {
    //TODO: implement it
};

ShapeType PolygonShape::GetType() const {
    return POLYGON;
};

Shape* PolygonShape::Clone() const {
    return new PolygonShape(localVertices);
};

float PolygonShape::GetMomentOfInertia() const {
    // TODO: implement it
    return 0.0;
};

void PolygonShape::UpdateVertices(float angle, const Vec2& position) const{
    // Loop all the vertices, transforming from local to world space
    for (int i = 0; i < localVertices.size(); i++) {
        // Must rotate first
        worldVertices[i] = localVertices[i].Rotate(angle);
        // Then translate
        worldVertices[i] += position;
    }
};

BoxShape::BoxShape(float width, float height) {
    this->width = width;
    this->height = height;

    float halfWidth = width / 2.0;
    float halfHeight = height / 2.0;

    //load the vertices of the box
    localVertices.push_back(Vec2(-halfWidth, -halfHeight));
    localVertices.push_back(Vec2(halfWidth, -halfHeight));
    localVertices.push_back(Vec2(halfWidth, halfHeight));
    localVertices.push_back(Vec2(-halfWidth, halfHeight));

    // Initialize world vertices
    worldVertices.push_back(Vec2(-halfWidth, -halfHeight));
    worldVertices.push_back(Vec2(halfWidth, -halfHeight));
    worldVertices.push_back(Vec2(halfWidth, halfHeight));
    worldVertices.push_back(Vec2(-halfWidth, halfHeight));
};

BoxShape::~BoxShape() {
    //TODO: implement it
};

ShapeType BoxShape::GetType() const {
    return BOX;
};

Shape* BoxShape::Clone() const {
    return new BoxShape(width, height);
};

float BoxShape::GetMomentOfInertia() const{
    // For solid rectangles the moment of inertia is 1/12 * (w^2 + h^2)
    // But this still needs to be multiplied by the mass of the RB
    return (1/12) * ((width * width) + (height * height));
};