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
    Particle* smallBall = new Particle(50, 100, 1.0, 15);
    particles.push_back(smallBall);

    Particle* bigBall = new Particle(Graphics::windowWidth - 50, 100, 3.0, 20);
    particles.push_back(bigBall);

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
                    
                    Particle* newParticle = new Particle(event.button.x, event.button.y, 1.0, 10);
                    particles.push_back(newParticle);
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

    // Apply forces to my particles
    for (auto particle : particles) {
        // Wind force if particle is not inside the fluid
        // if (particle->position.y < fluid.y) {
        //     Vec2 wind = Vec2(1.0 * PIXELS_PER_METER, 0.0 * PIXELS_PER_METER);
        //     particle->AddForce(wind);
        // }

        // Weight force
        // Vec2 weight = Vec2(0.0 * PIXELS_PER_METER, 9.8 * PIXELS_PER_METER * particle->mass);
        // particle->AddForce(weight);

        // Push force
        particle->AddForce(pushForce);

        // Apply a friction force
        Vec2 friction = Force::GenerateFrictionForce(*particle, 10.0 * PIXELS_PER_METER);
        particle->AddForce(friction);

        // Apply a drag force if particle is inside the fluid
        // if (particle->position.y >= fluid.y) {
        //     Vec2 drag = Force::GenerateDragForce(*particle, 0.04);
        //     particle->AddForce(drag);
        // }
    }

    // Integrate the acceleration and the velocity to find the new position
    for (auto particle : particles) {
        particle->Integrate(deltaTime);
    }

    // TODO: Check the particle position and try to keep the particle inside the boundaries
    // of the window
    for (auto particle : particles) {
        if ((particle->position.x + particle->radius) >= Graphics::windowWidth){
            particle->velocity.x *= -0.9;
            particle->position.x = Graphics::windowWidth - particle->radius;
        } else if (particle->position.x - particle->radius <= 0 ) {
            particle->velocity.x *= -0.9;
            particle->position.x = particle->radius;
        } 
        if ((particle->position.y + particle->radius) >= Graphics::windowHeight ) {
            particle->velocity.y *= -0.9;
            particle->position.y = Graphics::windowHeight - particle->radius;
        } else if(particle->position.y - particle->radius <= 0 ) {
            particle->velocity.y *= -0.9;
            particle->position.y = particle->radius;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
/////////////////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFFA1D2E6);

    // Draw the fluid in
    Graphics::DrawFillRect(fluid.x + fluid.w/2, fluid.y + fluid.h/2,fluid.w, fluid.h, 0xFF968035);
    for (auto particle : particles) {
        Graphics::DrawFillCircle(particle->position.x,particle->position.y,particle->radius,0xFFFFFFFF);
    }
    Graphics::RenderFrame();
}

/////////////////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
/////////////////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for(auto particle : particles) {
        delete particle;
    }

    Graphics::CloseWindow();
}