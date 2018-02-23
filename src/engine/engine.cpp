#include "engine.h"
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <stack>
#include <utils/logger.h>

bool Engine::createWindows(float width, float heigth) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return false;
    this->_window =
        SDL_CreateWindow("NoobEngine Pro-Alfa-Noob version 666", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         width, heigth, SDL_WINDOW_OPENGL);

    _mainContext = SDL_GL_CreateContext(this->_window);

#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    glewInit();
#endif
}
void Engine::resizeWindows(float width, float heigth) {}
void Engine::setWindowsVisible(bool status) {
    if (status)
        SDL_ShowWindow(this->_window);
    else
        SDL_HideWindow(this->_window);
}

void Engine::setScene(const std::shared_ptr<SceneNode> &newScene) {
    this->_scene = newScene;
}

Engine::~Engine() {
    LOG_INFO("Cleaning up SDL\n");
    SDL_GL_DeleteContext(_mainContext);
    SDL_DestroyWindow(this->_window);
    SDL_Quit();
}

void Engine::render() {

    if(this->_window == nullptr || this->getScene() == nullptr)
        return;

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    RenderState rs;
    rs.mvp = glm::mat4(1.0);
    rs.program = 0;
    this->getScene()->render(rs);

    SDL_GL_SwapWindow(this->_window);
}

void Engine::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT: {
            this->_isRunning = false;
        } break;
        }
        if(this->_messageCallback != nullptr)
            this->_messageCallback(event);
    }
}

void Engine::setMessageCallback(std::function<void(SDL_Event)> callback)
{
    assert(callback != nullptr);
    this->_messageCallback = callback;
}
