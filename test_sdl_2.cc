#include <iostream>
#include <string>

#include "SDL.h"
#include "SOIL.h"
#include "glad/glad.h"
#include "quez/camera.h"
#include "quez/game_object.h"
#include "quez/resource_manager.h"
#include "quez/sprite_renderer.h"

void logError(std::string errorString);

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    Uint32 flags = SDL_WINDOW_OPENGL;

    SDL_Window* window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 520, 620, flags);
    if (window == nullptr) {
        logError("Failed to create SDL window!");
    }

    //Set up our OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr) {
        logError("Failed to create SDL_GL context!");
    }

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        logError("Failed to load GL Loader!");
    }

    SDL_GL_SetSwapInterval(true);

    Shader shader = ResourceManager::loadShader("./resource/shader/sprite.vert", "./resource/shader/sprite.frag", nullptr, "Sprite");
    SpriteRenderer* renderer = new SpriteRenderer(shader);

    Texture2D bg = ResourceManager::loadTexture("./resource/sprite/background.png", false, "Background");

    Camera cam(520, 620);

    GameObject* bgObj = new GameObject(*renderer, bg, glm::vec3(520, 620, 0));

    cam.updateShaderViewProjection(shader);

    bool isRunning = true;

    while (isRunning) {
        SDL_Event evt;

        while (SDL_PollEvent(&evt)) {
            switch (evt.type) {
                case SDL_QUIT:
                    isRunning = false;
                    break;
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        bgObj->render();

        SDL_GL_SwapWindow(window);
    }

    return 0;
}

void logError(std::string errorString) {
    std::cout << errorString << std::endl;
    SDL_Quit();
    exit(1);
}