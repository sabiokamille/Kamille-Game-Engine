#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Vec2.h"
#include "./Physics/Force.h"
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
    Body* smallBall = new Body(CircleShape(50),500, 500, 1.0, 15);
    bodies.push_back(smallBall);

    // Body* bigBall = new Body(Graphics::windowWidth - 500, 100, 3.0, 20);
    // bodies.push_back(bigBall);

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
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_PRESSED) {
                    // std::cout << "x-coordinate: " << event.button.x << std::endl;
                    // std::cout << "y-coordinate: " << event.button.y << std::endl;
                    
                    Body* newBody = new Body(CircleShape(50),event.button.x, event.button.y, 3.0, 30);
                    bodies.push_back(newBody);
                }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update game objects)
/////////////////////////////////////////////////////////////////////////////////////////
void Application::Update() {
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

        // Push force
        Body->AddForce(pushForce);

        // Apply a friction force
        // Vec2 friction = Force::GenerateFrictionForce(*Body, 10.0 * PIXELS_PER_METER);
        // Body->AddForce(friction);

        // Apply a drag force if Body is inside the fluid
        if (Body->position.y >= fluid.y) {
            Vec2 drag = Force::GenerateDragForce(*Body, 0.01);
            Body->AddForce(drag);
        } else {
            Vec2 drag = Force::GenerateDragForce(*Body, 0.01);
            Body->AddForce(drag);
        }
    }

    // //Apply gravitational attraction force
    // Vec2 gravitationalAttractionForce = Force::GenerateAttractionForce(*bodies[0],*bodies[1], 1000.0 );
    // bodies[0]->AddForce(gravitationalAttractionForce);
    // bodies[1]->AddForce(-gravitationalAttractionForce);

    // Apply a spring force to each Body
    for (int i = 1; i < bodies.size(); i++) {
        Vec2 springForce = Force::GenerateSpringForce(*bodies[i], bodies[i-1]->position, 150, 10);
         bodies[i]->AddForce(springForce);
         bodies[i-1]->AddForce(-springForce);
    }
    
   

    // Integrate the acceleration and the velocity to find the new position
    for (auto Body : bodies) {
        Body->Integrate(deltaTime);
    }

    // TODO: Check the Body position and try to keep the Body inside the boundaries
    // of the window
    for (auto Body : bodies) {
        if ((Body->position.x + Body->radius) >= Graphics::windowWidth){
            Body->velocity.x *= -0.9;
            Body->position.x = Graphics::windowWidth - Body->radius;
        } else if (Body->position.x - Body->radius <= 0 ) {
            Body->velocity.x *= -0.9;
            Body->position.x = Body->radius;
        } 
        if ((Body->position.y + Body->radius) >= Graphics::windowHeight ) {
            Body->velocity.y *= -0.9;
            Body->position.y = Graphics::windowHeight - Body->radius;
        } else if(Body->position.y - Body->radius <= 0 ) {
            Body->velocity.y *= -0.9;
            Body->position.y = Body->radius;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
/////////////////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFFA1D2E6);

    // Draw the fluid in the screen
    // Graphics::DrawFillRect(fluid.x + fluid.w/2, fluid.y + fluid.h/2,fluid.w, fluid.h, 0xFF6E3712);

    //Draw the anchor for the spring on the screen
    // Graphics::DrawFillRect(500, 100,50,10, 0xFF7D7D7D);

    //Draw the spring connecting the anchor to the Body (bob)
    // Graphics::DrawLine(500,105,bodies[0]->position.x, bodies[0]->position.y, 0xFF000000);

    //Draw the spring connections between bodies
    for (int i = 1; i < bodies.size(); i++) {
            Graphics::DrawLine(bodies[i]->position.x,bodies[i]->position.y,bodies[i-1]->position.x, bodies[i-1]->position.y, 0xFF000000);
    }

    //Draw each Body on the screen
    for( auto Body : bodies) {
        Graphics::DrawFillCircle(Body->position.x,Body->position.y,Body->radius,0xFFFFFFFF);
        
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