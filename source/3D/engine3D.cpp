#include <3D.h>
#if ENABLE_3D_MODE

#include <string>

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <SDL/SDL_opengl.h>
#include <GL/glu.h>

#include <console.h>
#include <engine.h>
#include <color.h>

#include <3D/engine3D.h>

color Engine3D::clearColor = CLEAR_COLOR;

SDL_GLContext Engine3D::gContext;
GLuint Engine3D::glProgramID;
GLint Engine3D::gVertexPos2DLocation = -1;
GLuint Engine3D::gVBO = 0;
GLuint Engine3D::gIBO = 0;

bool initGL();
bool printProgramLog(GLuint program);
bool printShaderLog(GLuint shader);

bool Engine3D::init()
{
    Console::print("Initializing OpenGL");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    Engine3D::gContext = SDL_GL_CreateContext(Engine::window);
    if (Engine3D::gContext == NULL)
    {
        Console::print("Error: OpenGL couldn't be initialized.");
        Console::print(SDL_GetError());
        return false;
    }

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        Console::print("Warning: Unable to initialize GLEW");
        std::string temp = "";
        temp = (const char*)glewGetErrorString(glewError);
        Console::print(temp);
    }

    if (SDL_GL_SetSwapInterval(1) < 0)
    {
        Console::print("Warning: Unable to enable VSync.");
        Console::print(SDL_GetError());
    }

    if (!initGL())
    {
        Console::print("Error: OpenGL initialization failed");
        return false;
    }

    glClearColor(Engine3D::clearColor.r / 255.0f, Engine3D::clearColor.g / 255.0f, Engine3D::clearColor.b / 255.0f, Engine3D::clearColor.a / 255.0f);

    Console::print("OpenGL Initialized");
    return true;
}

bool initGL()
{
    Engine3D::glProgramID = glCreateProgram();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    const GLchar* vertexShaderSource[] =
    {
        "#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
    };

    glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

    glCompileShader(vertexShader);

    GLint vShaderCompiled = GL_FALSE;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
    if (vShaderCompiled != GL_TRUE)
    {
        Console::print("Error: Unable to compile vertex shader");
        printShaderLog(vertexShader);
        return false;
    }

    glAttachShader(Engine3D::glProgramID, vertexShader);

    // FRAGMENT SHADER
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* fragmentShaderSource[] =
    { "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }" };

    glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

    glCompileShader(fragmentShader);

    //Check fragment shader for errors
    GLint fShaderCompiled = GL_FALSE;
    glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled );
    if( fShaderCompiled != GL_TRUE )
    {
        Console::print("Error Unable to compile fragment shader");
        printShaderLog(fragmentShader);
        return false;
    }

    glAttachShader(Engine3D::glProgramID, fragmentShader);

    glLinkProgram(Engine3D::glProgramID);

    GLint programSuccess = GL_TRUE;
    glGetProgramiv( Engine3D::glProgramID, GL_LINK_STATUS, &programSuccess );
    if( programSuccess != GL_TRUE )
    {
        Console::print("Error: Unable to link program");
        printProgramLog(Engine3D::glProgramID);
        return false;
    }

    Engine3D::gVertexPos2DLocation = glGetAttribLocation(Engine3D::glProgramID, "LVertexPos2D");
    if (Engine3D::gVertexPos2DLocation == -1)
    {
        Console::print("LVertexPos2D is not a valid glsl program variable!");
        return false;
    }

    glClearColor(0, 0, 1, 1);

    GLfloat vertexData[] =
    {
        -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f,  0.5f,
        -0.5f,  0.5f
    };

    GLuint indexData[] = { 0, 1, 2, 3 };

    glGenBuffers(1, &Engine3D::gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, Engine3D::gVBO);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

    glGenBuffers(1, &Engine3D::gIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Engine3D::gIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLfloat), indexData, GL_STATIC_DRAW);

    return true;
}

bool printProgramLog(GLuint program)
{
    if (glIsProgram(program))
    {
        int logLength = 0;
        int maxLength = logLength;

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        
        char* infoLog = new char[maxLength];

        glGetProgramInfoLog(program, maxLength, &logLength, infoLog);
        if (logLength > 0) Console::print(infoLog);

        delete[] infoLog;
    }
    else
    {
        Console::print("Warning: Object is not a program\n");
        return false;
    }
    return true;
}
bool printShaderLog(GLuint shader)
{
    if (glIsShader(shader))
    {
        int logLength = 0;
        int maxLength = logLength;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        char* infoLog = new char[maxLength];

        glGetShaderInfoLog(shader, maxLength, &logLength, infoLog);
        if (logLength > 0) Console::print(infoLog);

        delete[] infoLog;
    }
    else
    {
        Console::print("Warning: Object is not a program\n");
        return false;
    }

    return true;
}

#endif