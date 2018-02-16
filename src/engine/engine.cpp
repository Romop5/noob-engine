#include "engine.h"
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <stack>
#include <utils/logger.h>

bool Engine::createWindows(float width, float heigth) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return false;
    this->_window =
        SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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

const std::shared_ptr<SceneNode> &Engine::getScene() {}

void Engine::render() {

    glClearColor(0.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 identity;
    this->_render(identity, this->_scene);

    SDL_GL_SwapWindow(this->_window);
}

void Engine::_render(const glm::mat4 &world, std::shared_ptr<SceneNode> &node) {
    glm::mat4 localTransform;
    switch (node->getNodeType()) {
    case SceneNodeType::SHADER: {
        auto program = std::dynamic_pointer_cast<SceneShader>(node);
        glUseProgram(program->getProgram());
    } break;
    case SceneNodeType::TRANSFORMATION: {
        auto transform = std::dynamic_pointer_cast<SceneTransform>(node);
        localTransform = localTransform * transform->getTransformation();
    } break;
    case SceneNodeType::VISUAL: {
        auto visual = std::dynamic_pointer_cast<SceneVisual>(node);
        for (auto &mesh : visual->getMeshes()) {
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, mesh.getVertexBufferObjectId());

            glVertexAttribPointer(0, // attribute 0. No particular reason for 0,
                                     // but must match the layout in the shader.
                                  3, // size
                                  GL_FLOAT, // type
                                  GL_FALSE, // normalized?
                                  0,        // stride
                                  (void *)0 // array buffer offset
            );
            // Draw the triangle !
            glDrawArrays(
                GL_TRIANGLES, 0,
                3); // Starting from vertex 0; 3 vertices total -> 1 triangle
            glDisableVertexAttribArray(0);
        }
    } break;
    default:
        {
        }
        //LOG_DEBUG("Unknown node type in renderer\n");
        
    }

    glm::vec4 position = glm::vec4(2.0, 0.0, 0.0, 0.0);

    position = position * world;

    //LOG_DEBUG("pos: %s\n", glm::to_string(position).c_str());

    //LOG_DEBUG("_render old transform: %s\n", glm::to_string(world).c_str());
    glm::mat4 newWorldTransform = world * localTransform;
    /*LOG_DEBUG("_render new transform: %s\n",
            glm::to_string(newWorldTransform).c_str());
    */
    for (auto &x : node->getChildren())
        _render(newWorldTransform, x);
}

void Engine::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT: {
            this->_isRunning = false;
        } break;
        }
    }
}
