#include "Application.h"
#include "./Physics/Constants.h"

bool Application::IsRunning() {
    return running;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Setup funciton (executed once in the beginning of the simulation)
/////////////////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    // TODDO: make this safer? using maybe a shared pointer or sumn
    particle = new Particle(50, 100, 1.0, 10);
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
                    break;
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
    
    // Proceed to update objects in the scene
    particle->acceleration = Vec2(2.0 * PIXELS_PER_METER, 9.8 * PIXELS_PER_METER);

    // Integrate the acceleration and the velocity to find the new position
    particle->Integrate(deltaTime);

    // TODO: Check the particle position and try to keep the particle inside the boundaries
    // of the window
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

/////////////////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
/////////////////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056236);
    Graphics::DrawFillCircle(particle->position.x,particle->position.y,particle->radius,0xFFFFFFFF);
    Graphics::RenderFrame();
}

/////////////////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
/////////////////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    delete particle;

    Graphics::CloseWindow();
}