#include "engine.h"
#include <utils/logger.h>
#include <stack>

void Engine::createWindows(float width, float heigth) {}
void Engine::resizeWindows(float width, float heigth) {}
void Engine::setWindowsVisible(bool status) {}
void Engine::setScene(const std::shared_ptr<SceneNode> &scene) {}

const std::shared_ptr<SceneNode> & Engine::getScene() {}

void Engine::render() {
    glm::vec4 identity;
    this->_render(identity, this->scene);  
}


void Engine::_render(const glm::vec4& world, std::shared_ptr<SceneNode>& node)
{
    glm::vec4 localTransform; 
    switch(node->getNodeType())
    {
        case SceneNodeType::TRANSFORMATION:
        {
            auto transform = std::dynamic_pointer_cast<SceneTransform>(node);
            localTransform = localTransform*transform->getTransformation();
        }
        break;
        default:
            LOG_DEBUG("Unknown node type in renderer\n");
    }

    glm::vec4 newWorldTransform = world*localTransform;
    for(auto &x: node->getChildren())
        _render(newWorldTransform, x); 
}
