#include <engine/engine.h>
#include <viewer/primitives.h>
#include "procgen.h"
class Generator
{

    ProcGen::Generation pg;
    public:
        bool compile(std::string filePath)
        {
            //pg.setDebugOn(false);

            if(this->pg.parseFile(filePath) == false)
                return false;
            if(this->pg.isReady())
            {
                return true;
            }
            return false;

        }
        bool generate(std::shared_ptr<SceneNode> parent)
        {
             if(this->pg.runInit() == false)
                    return false;

                this->pg.run(6);
                json result = this->pg.serialize();
                produceGeometryFromJson(parent, result);
                return true; 
        }
        void produceGeometryFromJson(std::shared_ptr<SceneNode> parent, json input)
        {

                LOG_DEBUG("Result of generation: \n%s\n",input.dump(1).c_str());

                LOG_DEBUG("Count of objects: %d\n", input.size());
                for(auto &symbol: input)
                {
              //      LOG_DEBUG("Object: %s\n", symbol.dump(1).c_str());
                    LOG_DEBUG("Type of symbol: %s\n",symbol["_type"].get<std::string>().c_str());
                    if(symbol["_type"].get<std::string>() == "cube")
                        processJsonCube(parent, symbol);
                }
        }

        void processJsonCube(std::shared_ptr<SceneNode> parent, json cube)
        {
            
            //LOG_DEBUG("Cube: %s\n", cube.dump(1).c_str());
            json jsonPosition = cube["position"];
            glm::vec3 position;
            position[0] = jsonPosition["x"].get<double>();
            position[1] = jsonPosition["y"].get<double>();
            position[2] = jsonPosition["z"].get<double>();
            double size = cube["sz"].get<double>();
            LOG_DEBUG("Read position: %s\n", glm::to_string(position).c_str());
            LOG_DEBUG("Size: %g\n", size);
            parent->addChild(this->createModelFromCube(position, size));
        }

        std::shared_ptr<SceneNode> createModelFromCube(glm::vec3 position, float sz)
        {
            auto model = std::make_shared<SceneVisual>();
            auto mesh = std::make_shared<Primitive>();
            mesh->createBox();
            model->appendMesh(mesh);
            auto transform = std::make_shared<SceneTransform>();
            glm::mat4 matrix = glm::scale(glm::vec3(sz))*glm::translate(position);
            transform->setTransformation(matrix);
            transform->addChild(model);
            return transform;
        }
};
