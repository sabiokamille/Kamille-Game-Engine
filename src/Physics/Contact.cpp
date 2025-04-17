#include "Contact.h"

void Contact::ResolvePenetration() {
    if (a->IsStatic() && b->IsStatic()) {
        return;
    }
    
    float da = depth / (a->invMass + b->invI) * a->invMass;
    float db = depth / (a->invMass + b->invI) * b->invMass;

    a->position -= normal * da;
    b->position += normal * db;
}