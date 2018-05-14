/**
 * @file ./viewer/main.cpp
 * @brief Entry point 
 * @copyright The MIT license 
 */

#include <engine/engine.h>
#include <glm/ext.hpp>
#include <iostream>
#include <utils/logger.h>

#include <thread>
#include <chrono>
#include <ratio>

#include <viewer/primitives.h>
#include <viewer/cameracontroller.h>
#include <viewer/generator.h>



int main(int argc, char** argv) {

    if(argc < 2)
    {
        LOG_INFO("USAGE: %s <generationFile>\n",argv[0]);
        exit(1);
    }

    size_t width = 1200;
    size_t height = 800;

    LOG_INFO("Starting viewer\n");

    auto cameraNode = std::make_shared<SceneCamera>();
    cameraNode->resize(width, height);

    auto nodeB = std::make_shared<SceneTransform>();
    cameraNode->addChild(nodeB);


    nodeB->setTransformation(glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.0,10.0)));

    /*
     * Set camera transform
     */
    glm::mat4 viewMatrix = glm::lookAt(
        glm::vec3(0, 0.0, -5.0), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));

    cameraNode->setViewTransform(viewMatrix);
    cameraNode->setPerspectiveTransform(45.0);


    /*
     * Create model
     */
    Engine engine;
    engine.setScene(cameraNode);
    engine.createWindows(width, height);

    LOG_INFO("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    /*
     * Group of objects
     */
    auto objects = std::make_shared<SceneNode>();

    /*
     * Create basic shader
     */
    auto shaderProgram = std::make_shared<ShaderProgram>();
    shaderProgram->LoadShaders("basic.vertex","basic.fragment");
    auto shaderNode = std::make_shared<SceneShader>(shaderProgram);

    /*
     * Create light
     */
    auto lightNode = std::make_shared<SceneLight>();
    lightNode->addChild(objects);
    shaderNode->addChild(lightNode);
    nodeB->addChild(shaderNode);

    size_t i = 0;

    // Show wireframed
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    //Set antialiasing/multisampling 
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST ); 
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST ); 
    glDisable( GL_LINE_SMOOTH ); 
    glDisable( GL_POLYGON_SMOOTH ); 
    glEnable( GL_MULTISAMPLE );

    size_t desiredFrameRate = 30;
    std::chrono::duration<double, std::milli> milisecondsPerPass {1000.0/desiredFrameRate};

    auto cameraController = std::make_shared<CameraController>(cameraNode);
    /*  REGISTER CALLBACK*/
    engine.setMessageCallback(
        [&cameraController](SDL_Event event) { cameraController->processMessage(event); }
    );


    /* GENERATION */
    
    if(1==1)
    {
    Generator gen;
        std::string name = argv[1];
        if(gen.compile(name.c_str()))
        {
            bool generationResult = false;
            generationResult = gen.generateGeometry(objects);
            if(generationResult == false)
                LOG_ERROR("Generation failed\n");
        } else {
            LOG_ERROR("Procedural compilation failed\n");
        }

    }


    /*  RENDERING LOOP */
    while (engine.isRunning()) {
        auto passStart = std::chrono::high_resolution_clock::now();

        engine.handleEvents();
        // Render
        engine.render();

        // calculate remaining time
        auto passEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = passEnd-passStart;
        std::this_thread::sleep_for(milisecondsPerPass-elapsed);
    }
}
