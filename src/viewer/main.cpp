#include <engine/engine.h>
#include <glm/ext.hpp>
#include <iostream>
#include <utils/logger.h>

int main() {

    size_t width = 800;
    size_t height = 600;
    LOG_INFO("Starting viewer\n");
    auto tree = std::make_shared<SceneNode>();

    auto node = std::make_shared<SceneTransform>();
    tree->addChild(node);

    auto nodeB = std::make_shared<SceneTransform>();
    node->addChild(nodeB);

    nodeB->setTransformation(glm::translate(glm::mat4(), glm::vec3(0.0,0.0,10.0)));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);
  
    glm::mat4 viewMatrix = glm::lookAt(
        glm::vec3(0, 0.0, -5.0), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));

    glm::mat4 camera = projection*viewMatrix;
    //glm::mat4 camera = viewMatrix;
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



    auto duplicTransf = std::make_shared<SceneTransform>();
   duplicTransf->setTransformation(glm::translate(glm::mat4(), glm::vec3(0.0,0.0,1.0)));
    duplicTransf->addChild(model);
    shaderNode->addChild(model);
    shaderNode->addChild(duplicTransf);
    nodeB->addChild(shaderNode);


    LOG_INFO("Json of tree: %s\n", engine.getScene()->to_json().dump(1).c_str());


    size_t i = 0;
    while (engine.isRunning()) {
        engine.handleEvents();
        engine.render();

        i++;
       glm::mat4 rot = glm::rotate(float(i)*0.001f, glm::vec3(0.0,1.0,0.0));
        nodeB->setTransformation(rot);
    }
}
