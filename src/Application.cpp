#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Vec2.h"
#include "./Physics/Force.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"
#include <iostream>

bool Application::IsRunning() {
    return running;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Setup funciton (executed once in the beginning of the simulation)
/////////////////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    // TODDO: make this safer? using maybe a shared pointer or sumn
    Body* smallBall = new Body(CircleShape(50),500, 500, 1.0);
    bodies.push_back(smallBall);

    Body* bigBall = new Body(CircleShape(100), Graphics::windowWidth - 500, 100, 3.0);
    bodies.push_back(bigBall);

    // for (int i = 1; i < 6; i++) {
    //     bodies.push_back(new Body(400 * i, 700, 1.0 * i, 15));
    // }

    fluid.x = 0;
    fluid.y = Graphics::Height() / 2;
    fluid.w = Graphics::Width();
    fluid.h = Graphics::Height()/2;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Input processing
/////////////////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
                if (event.key.keysym.sym == SDLK_UP) {
                    pushForce.y = -50 * PIXELS_PER_METER;
                }
                if (event.key.keysym.sym == SDLK_RIGHT) {
                    pushForce.x = 50 * PIXELS_PER_METER;
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    pushForce.y = 50 * PIXELS_PER_METER;
                }
                if (event.key.keysym.sym == SDLK_LEFT) {
                    pushForce.x = -50 * PIXELS_PER_METER;
                }
                    break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP) {
                    pushForce.y = 0;
                }
                if (event.key.keysym.sym == SDLK_RIGHT) {
                    pushForce.x = 0;
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                    pushForce.y = 0;
                }
                if (event.key.keysym.sym == SDLK_LEFT) {
                    pushForce.x = 0;
                }
                break;
            case SDL_MOUSEMOTION:
                int x,y;
                SDL_GetMouseState(&x, &y);
                bodies[1]->position.x = x;
                bodies[1]->position.y = y;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update game objects)
/////////////////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    // Clear screen so that debug info can be drawn and shown on screen
    Graphics::ClearScreen(0xFFA1D2E6);

    // Wait some time until we reach the target frame time in ms
    static int timePrevFrame;
    int waitTime = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePrevFrame);
    if (waitTime > 0) {
        SDL_Delay(waitTime);
    }

    // Calculate the deltaTime in seconds
    float deltaTime  = (SDL_GetTicks() - timePrevFrame) / 1000.0f;
    if(deltaTime > 0.016) deltaTime = 0.016;

    // Set the time of the current frame to be used in the next one;
    timePrevFrame = SDL_GetTicks();

    // Apply forces to my bodies
    for (auto Body : bodies) {
        // Wind force if Body is not inside the fluid
        // if (Body->position.y < fluid.y) {
        //     Vec2 wind = Vec2(1.0 * PIXELS_PER_METER, 0.0 * PIXELS_PER_METER);
        //     Body->AddForce(wind);
        // }

        // Weight force
        // Vec2 weight = Vec2(0.0, 9.8 * PIXELS_PER_METER * Body->mass);
        // Body->AddForce(weight);

        // Torque force
        // float torque = 20;
        // Body->AddTorque(torque);

        // Push force
        // Body->AddForce(pushForce);

        // Apply a friction force
        // Vec2 friction = Force::GenerateFrictionForce(*Body, 10.0 * PIXELS_PER_METER);
        // Body->AddForce(friction);

        // Apply a drag force if Body is inside the fluid
        // if (Body->position.y >= fluid.y) {
        //     Vec2 drag = Force::GenerateDragForce(*Body, 0.01);
        //     Body->AddForce(drag);
        // } else {
        //     Vec2 drag = Force::GenerateDragForce(*Body, 0.01);
        //     Body->AddForce(drag);
        // }
    }

    // Integrate the acceleration and the velocity to find the new position
    for (auto Body : bodies) {
        Body->Update(deltaTime);
    }

    // Reset the collision flag for all the bodies
    for (auto& Body : bodies) {
        Body->isColliding = false;
    }

    // Check all the rigid bodies with the other rigid bodies for collision
    for (int i = 0; i < bodies.size() - 1; i++) {
        for (int j = i+1; j < bodies.size(); j++) {
            Body* a = bodies[i];
            Body* b = bodies[j];

            Contact contact;
            if(CollisionDetection::IsColliding(a, b, contact)){
                // Resolve the collision using impulse method
                Graphics::DrawFillCircle(contact.pa.x, contact.pa.y, 3, 0xFFFF00FF);
                Graphics::DrawFillCircle(contact.pb.x, contact.pb.y, 3, 0xFFFF00FF);
                Graphics::DrawLine(contact.pa.x,contact.pa.y,(contact.pa.x + contact.normal.x * 15), (contact.pa.y + contact.normal.y * 15), 0xFFFF00FF);
                a->isColliding = true;
                b->isColliding = true;
            }
        }
    }

    // TODO: Check the Body position and try to keep the Body inside the boundaries
    // of the window
    for (auto Body : bodies) {
        if (Body->shape->GetType() == CIRCLE) {
            CircleShape* circleShape = (CircleShape*) Body->shape;
            if ((Body->position.x + circleShape->radius) >= Graphics::windowWidth){
            Body->velocity.x *= -0.9;
            Body->position.x = Graphics::windowWidth - circleShape->radius;
            } else if (Body->position.x - circleShape->radius <= 0 ) {
            Body->velocity.x *= -0.9;
            Body->position.x = circleShape->radius;
            } 
            if ((Body->position.y + circleShape->radius) >= Graphics::windowHeight ) {
            Body->velocity.y *= -0.9;
            Body->position.y = Graphics::windowHeight - circleShape->radius;
            } else if(Body->position.y - circleShape->radius <= 0 ) {
                Body->velocity.y *= -0.9;
                Body->position.y = circleShape->radius;
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
/////////////////////////////////////////////////////////////////////////////////////////
void Application::Render() {

    // Draw the fluid in the screen
    // Graphics::DrawFillRect(fluid.x + fluid.w/2, fluid.y + fluid.h/2,fluid.w, fluid.h, 0xFF6E3712);

    //Draw each Body on the screen
    for( auto Body : bodies) {
        Uint32 color = Body->isColliding? 0xFF0000FF : 0xFFFFFFFF;
        if (Body->shape->GetType() == CIRCLE) {
            CircleShape* circleShape = (CircleShape*) Body->shape;
            Graphics::DrawCircle(Body->position.x,Body->position.y,circleShape->radius,Body->rotation, color);
        } else if (Body->shape->GetType() == BOX) {
            BoxShape* boxShape = (BoxShape*) Body->shape;
            Graphics::DrawPolygon(Body->position.x, Body->position.y, boxShape->worldVertices, 0xFFFFFFFF );
        } else {
            // TODO: Draw other types of shapes
        }
        
    }

    Graphics::RenderFrame();
}

/////////////////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
/////////////////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for(auto Body : bodies) {
        delete Body;
    }

    Graphics::CloseWindow();
}