#include <iostream>
#include <thread>
#include <math.h>

#include "SDL/SDL.h"

#include "global.h"
#include "engine.h"
#include "event.h"
#include "script.h"
#include "font.h"
#include "console.h"
#include "render.h"

#include <3D.h>

#if ENABLE_3D_MODE
#include <3D/engine3D.h>
#endif

long prevTime = 0;

SDL_Window* Engine::window = NULL;
SDL_Renderer* Engine::renderer = NULL;

bool Engine::quit = false;
long Engine::currentTime = 0;
long Engine::deltaTime = 0;

bool Engine::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Could not initialize SDL, Error: " << SDL_GetError();
        return false;
    }
    else
    {
        SDL_CreateWindowAndRenderer(
            Global::windowWidth, Global::windowHeight, 0,
            &window, &renderer
        );
        if (window == NULL)
        {
            std::cout << "Window could not be created, Error: " << SDL_GetError();
            return false;
        }
        else
        { }
    }
    return true;
}
void Engine::close()
{
    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}
void Engine::start()
{
    if (!Font::init())
    {
        std::cout << "Failed to initialize fonts";
        return;
    }

#if ENABLE_3D_MODE
    Engine3D::init();
#endif

    Script::start();

    std::thread eventThread(Event::init);
    std::thread timerThread(timerUpdate);

    while (!Engine::quit)
    {
        if (Global::fpsLimit != 0)
        {
            while (Engine::currentTime - prevTime <= 1000.0 / Global::fpsLimit);
        }
        Engine::deltaTime = Engine::currentTime - prevTime;
        prevTime = Engine::currentTime;
        Engine::update();
    }
}

float pos = 0;

int fpsTimer = 500;
float fpsRead = 0;
void Engine::update()
{
#if ENABLE_3D_MODE
    glClear(GL_COLOR_BUFFER_BIT);
#endif

    Script::update();
    Console::update();

    if (Global::fpsOutput)
    {
        if (fpsTimer > 500)
        {
            fpsRead = 1000.0 / Engine::deltaTime;
            fpsTimer -= 500;
        }

        Render::fillRect(0, Global::windowHeight - 25, 150, 25, { 255, 255, 255, 255 });
        Render::setColor({ 0, 0, 0, 255 });
        Render::drawText(10, Global::windowHeight - 20, // draw fps output
            Font::consolas,
            std::to_string(fpsRead) + " fps",
            0.7
        );

        fpsTimer += Engine::deltaTime;
    }

    glColor3f(1.0, 0, 0);
    glBegin(GL_QUADS);
        glVertex2f(cosf32(pos) * 0.05, sinf32(pos) * 0.05);
        glVertex2f(cosf32(pos + 1) * 0.05, sinf32(pos + 1) * 0.05);
        glVertex2f(cosf32(pos + 0.51f) * 0.95, sinf32(pos + 0.51f) * 0.95);
        glVertex2f(cosf32(pos + 0.49f) * 0.95, sinf32(pos + 0.49f) * 0.95);
    glEnd();

    pos += 0.001f * Engine::deltaTime;

    Render::present();
}
void Engine::timerUpdate()
{
    while (true)
    {
        currentTime++;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}