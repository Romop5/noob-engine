#include "engine.h"
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <stack>
#include <utils/logger.h>

bool Engine::createWindows(float width, float heigth) {



    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
        return false;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
 
    this->_window =
        SDL_CreateWindow("NoobEngine Pro-Alfa-Noob version 666", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         width, heigth, SDL_WINDOW_OPENGL);

    SDL_SetWindowResizable(this->_window, SDL_TRUE);
    _mainContext = SDL_GL_CreateContext(this->_window);

    // init GUI
    _gui.init(this->_window);


#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    glewInit();
#endif
}
void Engine::resizeWindows(float width, float heigth) 
{
    if(this->getScene() != nullptr)
    {
        LOG_INFO("Resize %fx%f\n", width, heigth);
        auto cameraNode = std::dynamic_pointer_cast<SceneCamera>(this->getScene());
        cameraNode->resize(width, heigth);
        glViewport(0.0,0.0,width, heigth);
    }
}
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
    // Render scene
    this->getScene()->render(rs);

    // render GUI
    _gui.render();

    SDL_GL_SwapWindow(this->_window);
}

void Engine::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        LOG_INFO("Fucking messeage\n");
        switch (event.type) {
            case SDL_QUIT: {
                this->_isRunning = false;
            } break;
            case SDL_WINDOWEVENT: 
            {
                switch(event.window.event)
                {
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED: {
                        this->resizeWindows(event.window.data1, event.window.data2);
                    } break;
                }
            }
        }
        if(this->_messageCallback != nullptr)
            this->_messageCallback(event);
        this->_gui.handleInput(event);
    }
}

void Engine::setMessageCallback(std::function<void(SDL_Event)> callback)
{
    assert(callback != nullptr);
    this->_messageCallback = callback;
}
