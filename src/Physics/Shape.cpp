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
    return new PolygonShape(vertices);
};

BoxShape::BoxShape(float width, float height) {
    // TODO: implment it
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