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
    // Particle* smallBall = new Particle(500, 500, 1.0, 15);
    // particles.push_back(smallBall);

    // Particle* bigBall = new Particle(Graphics::windowWidth - 500, 100, 3.0, 20);
    // particles.push_back(bigBall);

    // for (int i = 1; i < 5; i++) {
    //     particles.push_back(new Particle((400 + 25 * (i % 2)), (700 + 25 * (i % 2)), 1.0 * i, 15));
    // }

    particles.push_back(new Particle(400,700,1.0,15));
    particles.push_back(new Particle(500,700,1.0,15));
    particles.push_back(new Particle(500,800, 1.0,15));
    particles.push_back(new Particle(400,800,1.0,15));

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
                    
                    Particle* newParticle = new Particle(event.button.x, event.button.y, 3.0, 30);
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
        // Vec2 weight = Vec2(0.0, 9.8 * PIXELS_PER_METER * particle->mass);
        // particle->AddForce(weight);

        // Push force
        particle->AddForce(pushForce);

        // Apply a friction force
        // Vec2 friction = Force::GenerateFrictionForce(*particle, 10.0 * PIXELS_PER_METER);
        // particle->AddForce(friction);

        // Apply a drag force if particle is inside the fluid
        if (particle->position.y >= fluid.y) {
            Vec2 drag = Force::GenerateDragForce(*particle, 0.01);
            particle->AddForce(drag);
        } else {
            Vec2 drag = Force::GenerateDragForce(*particle, 0.01);
            particle->AddForce(drag);
        }
    }

    // //Apply gravitational attraction force
    // Vec2 gravitationalAttractionForce = Force::GenerateAttractionForce(*particles[0],*particles[1], 1000.0 );
    // particles[0]->AddForce(gravitationalAttractionForce);
    // particles[1]->AddForce(-gravitationalAttractionForce);

    // Apply a spring force to each particle
    for (int i = (particles.size() - 1); i > 0; i--) {
        for (int j = 0; j < i; j++) {
        Vec2 springForce = Force::GenerateSpringForce(*particles[i], particles[j]->position, 150, 10);
         particles[i]->AddForce(springForce);
         particles[j]->AddForce(-springForce);
        }
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

    // Draw the fluid in the screen
    // Graphics::DrawFillRect(fluid.x + fluid.w/2, fluid.y + fluid.h/2,fluid.w, fluid.h, 0xFF6E3712);

    //Draw the anchor for the spring on the screen
    // Graphics::DrawFillRect(500, 100,50,10, 0xFF7D7D7D);

    //Draw the spring connecting the anchor to the particle (bob)
    // Graphics::DrawLine(500,105,particles[0]->position.x, particles[0]->position.y, 0xFF000000);

    //Draw the spring connections between particles
    for (int i = (particles.size() - 1); i > 0; i--) {
        for (int j = 0; j < i; j++) {
            Graphics::DrawLine(particles[i]->position.x,particles[i]->position.y,particles[j]->position.x, particles[j]->position.y, 0xFF000000);
        }
    }
            

    //Draw each particle on the screen
    for( auto particle : particles) {
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