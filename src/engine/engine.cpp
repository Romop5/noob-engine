#include "engine.h"
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <stack>
#include <utils/logger.h>

void Engine::createWindows(float width, float heigth) {}
void Engine::resizeWindows(float width, float heigth) {}
void Engine::setWindowsVisible(bool status) {}
void Engine::setScene(const std::shared_ptr<SceneNode> &newScene) {
    this->scene = newScene;
}

const std::shared_ptr<SceneNode> &Engine::getScene() {}

void Engine::render() {
    glm::mat4 identity;
    this->_render(identity, this->scene);
}

void Engine::_render(const glm::mat4 &world, std::shared_ptr<SceneNode> &node) {
    glm::mat4 localTransform;
    switch (node->getNodeType()) {
    case SceneNodeType::TRANSFORMATION: {
        auto transform = std::dynamic_pointer_cast<SceneTransform>(node);
        localTransform = localTransform * transform->getTransformation();
    } break;
    default:
        LOG_DEBUG("Unknown node type in renderer\n");
    }

    glm::vec4 position = glm::vec4(2.0, 0.0, 0.0, 0.0);

    position = position * world;

    LOG_DEBUG("pos: %s\n", glm::to_string(position).c_str());

    LOG_DEBUG("_render old transform: %s\n", glm::to_string(world).c_str());
    glm::mat4 newWorldTransform = world * localTransform;
    LOG_DEBUG("_render new transform: %s\n",
              glm::to_string(newWorldTransform).c_str());
    for (auto &x : node->getChildren())
        _render(newWorldTransform, x);
}
