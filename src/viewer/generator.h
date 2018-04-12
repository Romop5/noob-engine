#include <engine/engine.h>
#include <engine/scenevisual.h>
#include <engine/mesh.h>
#include <viewer/primitives.h>
#include <procgen/procgen.h>
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
        bool generateCubes(std::shared_ptr<SceneNode> parent)
        {
             if(this->pg.runInit() == false)
                    return false;

            this->pg.run(0);
            json result = this->pg.serialize();
            produceCubesFromJson(parent, result);
            return true; 
        }
        bool generate()
        {
             if(this->pg.runInit() == false)
                    return false;

            this->pg.run(0);
            return true;
        }

        bool produceOutput(std::shared_ptr<SceneNode> parent)
        {
            json result = this->pg.serialize();
            produceCubesFromJson(parent, result);
            return true; 
        }
 
        bool generate(std::shared_ptr<SceneNode> parent)
        {
             if(this->pg.runInit() == false)
                    return false;

            this->pg.run(0);
            json result = this->pg.serialize();
            produceGeometryFromJson(parent, result);
            return true; 
        }

        void produceGeometryFromJson(std::shared_ptr<SceneNode> parent, json input)
        {
                std::vector<Triangle> geometry;
        
                LOG_DEBUG("Result of generation: \n%s\n",input.dump(1).c_str());
                LOG_DEBUG("Count of objects: %d\n", input.size());
                for(auto &symbol: input)
                {
                    if(symbol["_type"].get<std::string>() == "triangle")
                    {
                        auto vertices = symbol["vertices"];
                        std::vector<Vertex> verticesForTriangle;
                        for(auto &vertex: vertices)
                        {
                            if(vertex["_type"].get<std::string>() == "vertex")
                            {
                                auto pos = vertex["position"];
                                glm::vec3 position;
                                position = glm::vec3(pos["x"].get<double>(),pos["y"].get<double>(), pos["z"].get<double>());
                                verticesForTriangle.push_back(Vertex(position));
                            }
                        }
                        geometry.push_back({
                                verticesForTriangle[0],
                                verticesForTriangle[1],
                                verticesForTriangle[2]
                        });
                    }
                }
                auto mesh = std::make_shared<Mesh>();
                mesh->createFromVertices(geometry,
                    VertexAtributes::NORMAL|VertexAtributes::POSITION); 
                auto model = std::make_shared<SceneVisual>();
                model->appendMesh(mesh);
                parent->addChild(model);
        }

        void produceCubesFromJson(std::shared_ptr<SceneNode> parent, json input)
        {

                LOG_DEBUG("Result of generation: \n%s\n",input.dump(1).c_str());

                LOG_DEBUG("Count of objects: %d\n", input.size());
                for(auto &symbol: input)
                {
                    LOG_DEBUG("Object: %s\n", symbol.dump(1).c_str());
                    LOG_DEBUG("Type of symbol: %s\n",symbol["_type"].get<std::string>().c_str());
                    if(symbol["_type"].get<std::string>() == "cube")
                        processJsonCube(parent, symbol);

                    if(symbol["_type"].get<std::string>() == "block")
                        processJsonBlock(parent, symbol);
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

            glm::vec3 color = glm::vec3(0.1,0.1,0.1);
            json jsonColor = cube["color"];
            if(cube["color"] != nullptr)
            {
                color[0] = jsonColor["x"].get<double>();
                color[1] = jsonColor["y"].get<double>();
                color[2] = jsonColor["z"].get<double>();
            }

            glm::mat4 rotation = glm::mat4(1.0);
            if(cube.find("rotation") != cube.end())
            {
                // load rotation
                rotation[0][0] = cube["rotation"]["a"]["x"];
                rotation[0][1] = cube["rotation"]["a"]["y"];
                rotation[0][2] = cube["rotation"]["a"]["z"];
                rotation[0][3] = 0.0;

                rotation[1][0] = cube["rotation"]["b"]["x"];
                rotation[1][1] = cube["rotation"]["b"]["y"];
                rotation[1][2] = cube["rotation"]["b"]["z"];
                rotation[1][3] = 0.0;

                rotation[2][0] = cube["rotation"]["c"]["x"];
                rotation[2][1] = cube["rotation"]["c"]["y"];
                rotation[2][2] = cube["rotation"]["c"]["z"];
                rotation[2][3] = 0.0;

                rotation[3] = glm::vec4(0.0,0.0,0.0,1.0);
            }

            LOG_DEBUG("Read position: %s\n", glm::to_string(position).c_str());
            LOG_DEBUG("Size: %g\n", size);

            parent->addChild(this->createModelFromCube(position,rotation, glm::vec3(size),color));

        }

        void processJsonBlock(std::shared_ptr<SceneNode> parent, json block)
        {
            
            //LOG_DEBUG("Cube: %s\n", cube.dump(1).c_str());
            json jsonPosition = block["position"];
            glm::vec3 position;
            position[0] = jsonPosition["x"].get<double>();
            position[1] = jsonPosition["y"].get<double>();
            position[2] = jsonPosition["z"].get<double>();

            glm::vec3 color = glm::vec3(0.1,0.1,0.1);
            json jsonColor = block["color"];
            if(block["color"] != nullptr)
            {
                color[0] = jsonColor["x"].get<double>();
                color[1] = jsonColor["y"].get<double>();
                color[2] = jsonColor["z"].get<double>();
            }

            glm::vec3 size  = glm::vec3(1.0,1.0,0.1);
            json jsonSize = block["sz"];
            if(block["sz"] != nullptr)
            {
                size[0] = jsonSize["x"].get<double>();
                size[1] = jsonSize["y"].get<double>();
                size[2] = jsonSize["z"].get<double>();
            }

            glm::mat4 rotation = glm::mat4(1.0);
            if(block.find("rotation") != block.end())
            {
                // load rotation
                rotation[0][0] = block["rotation"]["a"]["x"];
                rotation[0][1] = block["rotation"]["a"]["y"];
                rotation[0][2] = block["rotation"]["a"]["z"];
                rotation[0][3] = 0.0;

                rotation[1][0] = block["rotation"]["b"]["x"];
                rotation[1][1] = block["rotation"]["b"]["y"];
                rotation[1][2] = block["rotation"]["b"]["z"];
                rotation[1][3] = 0.0;

                rotation[2][0] = block["rotation"]["c"]["x"];
                rotation[2][1] = block["rotation"]["c"]["y"];
                rotation[2][2] = block["rotation"]["c"]["z"];
                rotation[2][3] = 0.0;

                rotation[3] = glm::vec4(0.0,0.0,0.0,1.0);
            }

            LOG_DEBUG("Read position: %s\n", glm::to_string(position).c_str());
            LOG_DEBUG("Read color : %s\n", glm::to_string(color).c_str());
            LOG_DEBUG("Read size: %s\n", glm::to_string(size).c_str());
            parent->addChild(this->createModelFromCube(position,rotation, size,color));
        }




        std::shared_ptr<SceneNode> createModelFromCube(glm::vec3 position, glm::mat4 rotation, glm::vec3 sz, glm::vec3 color)
        {
            auto model = std::make_shared<SceneVisual>();
            auto mesh = std::make_shared<Primitive>();
            mesh->createBox(color);
            model->appendMesh(mesh);
            auto transform = std::make_shared<SceneTransform>();
            //glm::mat4 matrix = glm::scale(glm::vec3(sz*0.5))*glm::translate(position);
            //glm::mat4 matrix = glm::scale(sz)*glm::translate(position);
            //
            
            LOG_INFO("Rotation : %s\n", glm::to_string(rotation).c_str());

            //rotation = glm::rotate(glm::mat4(1.0f), 30.0f, glm::vec3(1.0f,1.0f,1.0f));

            glm::mat4 matrix = glm::translate(position)*glm::transpose(rotation)*glm::scale(sz);
            //glm::mat4 matrix = rotation;//*glm::scale(sz);
            transform->setTransformation(matrix);
            transform->addChild(model);
            return transform;
        }
};
