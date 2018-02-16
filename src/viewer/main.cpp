#include <engine/engine.h>
#include <glm/ext.hpp>
#include <iostream>
#include <utils/logger.h>

int main() {

    LOG_INFO("Starting viewer\n");
    auto tree = std::make_shared<SceneNode>();

    auto node = std::make_shared<SceneTransform>();
    tree->addChild(node);

    auto nodeB = std::make_shared<SceneTransform>();
    node->addChild(nodeB);

    node->setTransformation(glm::lookAt(
        glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0)));

    LOG_INFO("Json of tree: %s\n", tree->to_json().dump(1).c_str());

    /*
     * Create model
     */
    Engine engine;
    engine.setScene(tree);
    engine.createWindows(800, 600);

    auto model = std::make_shared<SceneVisual>();
    Mesh mesh;

    static const std::vector<Vertex> triangle = {{glm::vec3(-1.0f, -1.0f, 0.0f),
                                                  glm::vec3(1.0f, -1.0f, 0.0f),
                                                  glm::vec3(0.0f, 1.0f, 0.0f)}};
    mesh.createFromVertices(triangle);
    model->appendMesh(mesh);
    nodeB->addChild(model);

    while (engine.isRunning()) {
        engine.handleEvents();
        engine.render();
    }
}
