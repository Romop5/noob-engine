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

    glm::mat4 perspective = glm::perspectiveFov(
                            40.0, 800.0,600.0,0.0,100.0);
    glm::mat4 viewMatrix = glm::lookAt(
        glm::vec3(0, 0.0, -10.0), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));

    glm::mat4 camera = viewMatrix*perspective;
    node->setTransformation(camera);


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

    auto shaderProgram = std::make_shared<ShaderProgram>();
    shaderProgram->LoadShaders("basic.vertex","basic.fragment");
    auto shaderNode = std::make_shared<SceneShader>(shaderProgram);

    shaderNode->addChild(model);
    nodeB->addChild(shaderNode);


    LOG_INFO("Json of tree: %s\n", engine.getScene()->to_json().dump(1).c_str());

    while (engine.isRunning()) {
        engine.handleEvents();
        engine.render();
    }
}
