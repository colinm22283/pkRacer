#include <math.h>
#include <iostream>

#include <global.h>
#include <script.h>
#include <engine.h>
#include <console.h>
#include <render.h>
#include <input.h>
#include <consoleParser.h>
#include <font.h>

#include <img/bmp.h>

void Script::exit()
{
    Console::print("Exit");

    Engine::quit = true;
}

void Script::keyDown(SDL_Keysym keysym) { }

void Script::start()
{
    Global::fpsLimit = 1000;
    //Global::fpsOutput = true;

    
}
void Script::update()
{
    
}

void Script::mouseDown(int button) { }
void Script::mouseClick(int button) { }

bool Script::setVar(std::string var, std::string value) { return false; }
bool Script::command(std::string cmd, std::string params) { return false; }