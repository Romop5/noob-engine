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


#include <sys/types.h>
#include <dirent.h>
//#include <libgen.h>
#include <algorithm>


#include <viewer/scriptcontrol.h>

std::vector<std::string> getFilesInDirectory(std::string path)
{
    DIR *dirp;
    struct dirent *dp;
    std::vector<std::string> files;


    if ((dirp = opendir(".")) == NULL) {
        LOG_ERROR("couldn't open '.'");
        return files;
    }


    do {
        errno = 0;
        if ((dp = readdir(dirp)) != NULL) {
            files.push_back(dp->d_name);
        }
    } while (dp != NULL);


    if (errno != 0)
        LOG_ERROR("error reading directory");
    (void) closedir(dirp);

    // filter out all files except those containing "example"
    files.erase(std::remove_if(files.begin(), files.end(), [](std::string& item){return (item.find("example") == std::string::npos);}),
                           files.end());

    // sort files
    std::sort(files.begin(), files.end());
    return files;
}


int main(int argc, char** argv) {

   size_t width = 1200;
    size_t height = 800;

    LOG_INFO("Starting viewer\n");

    auto cameraNode = std::make_shared<SceneCamera>();

    auto nodeB = std::make_shared<SceneTransform>();
    cameraNode->addChild(nodeB);


    nodeB->setTransformation(glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.0,10.0)));

	LOG_INFO("nodeB: %s\n", glm::to_string(nodeB->getTransformation()).c_str());
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 1000.0f);
  
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
    
 //   return 1;

    auto objects = std::make_shared<SceneNode>();

    auto shaderProgram = std::make_shared<ShaderProgram>();
    shaderProgram->LoadShaders("basic.vertex","basic.fragment");
    auto shaderNode = std::make_shared<SceneShader>(shaderProgram);

  //  shaderNode->addChild(model);
    auto lightNode = std::make_shared<SceneLight>();
    lightNode->addChild(objects);
    shaderNode->addChild(lightNode);
    nodeB->addChild(shaderNode);

    size_t i = 0;

    // Show wireframed
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    size_t desiredFrameRate = 30;
    std::chrono::duration<double, std::milli> milisecondsPerPass {1000.0/desiredFrameRate};

    auto cameraController = std::make_shared<CameraController>(cameraNode);
    /*  REGISTER CALLBACK*/
    engine.setMessageCallback(
        [&cameraController](SDL_Event event) { cameraController->processMessage(event); }
    );


    /* GENERATION */
    
    /*
    if(1==1)
    {
    Generator gen;
        std::string name = argv[1];
        if(gen.compile(name.c_str()))
        {
            bool generationResult = false;
            if(name.find("cube") != std::string::npos)
            {
                generationResult = gen.generateCubes(objects);
            } else {
                generationResult = gen.generate(objects);
            }
            if(generationResult == false)
                LOG_ERROR("Generation failed\n");
        } else {
            LOG_ERROR("Procedural compilation failed\n");
        }

    }
    */

    //if(engine.getScene() != nullptr)
      //  LOG_INFO("Json of tree: %s\n", engine.getScene()->to_json().dump(1).c_str());

    auto scriptmachine = std::make_shared<ScriptControl>();
    scriptmachine->init(); 
   
    bool shouldRotateModel = true;
    auto files = getFilesInDirectory("."); 
    engine.getGUI().registerCallback(
		    [files,&scriptmachine,&shouldRotateModel] {

                static int iterationCount = 0;
                static float stochasticity = 0.5;
                static bool open = true;
                ImGui::Begin("Editor", &open, ImGuiWindowFlags_AlwaysAutoResize);
                ImGui::Text("Choose the script you want to show. ");
                for(auto &file: files)
                {
                    if(file.find("gen") != std::string::npos)
                        if(ImGui::Selectable(file.c_str()))
                        {
                          scriptmachine->addScript(file, iterationCount,stochasticity);
                        }
                }
                
                ImGui::Separator();
                ImGui::Checkbox("Rotate model", &shouldRotateModel);
                ImGui::SliderInt("Maximum iterations",&iterationCount,1, 8); 
                ImGui::SliderFloat("Rate of stochasticity",&stochasticity,0.0, 1.0); 
                
                if(ImGui::Button("Generate"))
                {
                    scriptmachine->addScript(scriptmachine->getLastScriptName(), iterationCount, stochasticity);
                }


                ImGui::End();
              
                if(scriptmachine->hasJob())
                    ImGui::OpenPopup("Loading"); 
                if(ImGui::BeginPopupModal("Loading"))
                {
                    if(!scriptmachine->hasJob())
                        ImGui::CloseCurrentPopup();
                    ImGui::Text("Wait till loading finishes");
                    ImGui::EndPopup();
                }
                
                
            }
		    );
    /*  RENDERING LOOP */
    while (engine.isRunning()) {
        auto passStart = std::chrono::high_resolution_clock::now();

        scriptmachine->tick(objects);
        engine.handleEvents();
        // Render
        engine.render();

        if(shouldRotateModel)
        {
            // Do rotation
            i++;
            glm::mat4 rot = glm::rotate(float(i)*0.01f, glm::vec3(0.0,1.0,0.0));
            nodeB->setTransformation(rot);
        }

        // calculate remaining time
        auto passEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = passEnd-passStart;
        std::this_thread::sleep_for(milisecondsPerPass-elapsed);
    }
	return 1;
}
