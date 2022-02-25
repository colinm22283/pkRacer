#pragma once

#include <3D.h>
#if ENABLE_3D_MODE

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <color.h>

namespace Engine3D
{
    extern color clearColor;

    extern SDL_GLContext gContext;
    extern GLuint glProgramID;
    extern GLint gVertexPos2DLocation;
    extern GLuint gVBO;
    extern GLuint gIBO;

    bool init();
}

#endif