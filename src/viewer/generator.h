/**
 * @file ./viewer/generator.h
 * @brief Wrapper for ProcGen which keeps results and carry out visuals
 * @copyright The MIT license 
 */

#include <engine/engine.h>
#include <engine/scenevisual.h>
#include <engine/mesh.h>
#include <viewer/primitives.h>
#include <procgen/procgen.h>


#include "bitmap_image.hpp"
class Generator
{

    ProcGen::Procgen pg;

    std::vector<Triangle> triangles;

    bool shouldMergeTriangles;
    public:
        ProcGen::Procgen& getLibrary() { return this->pg;}
        bool compile(std::string filePath)
        {
			LOG_INFO("Compiling %s\n", filePath.c_str());
            shouldMergeTriangles = true;
            //pg.setDebugOn(false);

            if(this->pg.parseFile(filePath) == false)
                return false;
            if(this->pg.isReady())
            {
                return true;
            }
            return false;

        }

        bool generate()
        {
             if(this->pg.runInit() == false)
                    return false;

			LOG_INFO("Starting generation process.\n");
            this->pg.run(0);
			LOG_INFO("Generation done.\n");
            return true;
        }

        bool produceOutput(std::shared_ptr<SceneNode> parent)
        {
			LOG_INFO("Producing visuals from JSON output.\n");
            produceGeometryFromJson(parent, this->pg);

            if(shouldMergeTriangles)
            {
                auto mesh = std::make_shared<Mesh>();
                mesh->createFromVertices(triangles,VertexAtributes::POSITION | VertexAtributes::COLOR
                | VertexAtributes::NORMAL);
                auto visual = std::make_shared<SceneVisual>();
                visual->appendMesh(mesh);
                parent->addChild(visual);
            }
            return true; 
        }
        bool generateGeometry(std::shared_ptr<SceneNode> parent)
        {
            if(this->generate() == false)
                return false;
        
            return produceOutput(parent);    
        }

 
        void produceGeometryFromJson(std::shared_ptr<SceneNode> parent, ProcGen::Procgen& procgen)
        {
              size_t countOfObjects = procgen.countOfSymbols();
              for(size_t i = 0; i < countOfObjects; i++)
                {
                    json symbol = procgen.at(i);
                    LOG_DEBUG("Object: %s\n", symbol.dump(1).c_str());
                    LOG_DEBUG("Type of symbol: %s\n",symbol["_type"].get<std::string>().c_str());

                    // Cube is presented as vec3 position, float size and color
                    if(symbol["_type"].get<std::string>() == "cube")
                        processJsonCube(parent, symbol);

                    // Block is generalized cube with is presented as vec3 position, vec3 size and color
                    if(symbol["_type"].get<std::string>() == "block")
                        processJsonBlock(parent, symbol);

                    // Polygon is a set of 3D points and vec3 color, defining a mesh
                    if(symbol["_type"].get<std::string>() == "polygon")
                        processPolygon(parent, symbol);
                }
        }

        void processPolygon(std::shared_ptr<SceneNode> parent, json polygon)
        {
            // each polygon has a collection with name 'points'
            if(polygon.find("points") == polygon.end())
            {
                return;
            }

            std::vector<glm::vec3> points;

            for(auto point: polygon["points"])
            {
                points.push_back(glm::vec3(point["x"].get<float>(),point["y"].get<float>(),point["z"].get<float>()));
            }

            glm::vec3 color = glm::vec3(polygon["color"]["x"].get<float>(),polygon["color"]["y"].get<float>(),polygon["color"]["z"].get<float>());

            parent->addChild(this->createPolygon(points,color));
        }

        void processJsonCube(std::shared_ptr<SceneNode> parent, json cube)
        {
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


            if(this->shouldMergeTriangles)
            {
                auto anotherTriangles= this->createModelFromCubeVector(position, rotation,glm::vec3(size),color);
                this->triangles.insert(this->triangles.end(), anotherTriangles.begin(), anotherTriangles.end());

            } else {
                parent->addChild(this->createModelFromCube(position,rotation, glm::vec3(size),color));
            }
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

            if(this->shouldMergeTriangles)
            {
                auto anotherTriangles = this->createModelFromCubeVector(position,rotation, size,color);
                this->triangles.insert(this->triangles.end(), anotherTriangles.begin(), anotherTriangles.end());
            } else {
                parent->addChild(this->createModelFromCube(position,rotation, size,color));
            }
        }


        std::shared_ptr<SceneNode> createPolygon(const std::vector<glm::vec3>& points, const glm::vec3& color)
        {
            auto model = std::make_shared<SceneVisual>();
            auto mesh = std::make_shared<Primitive>();
            mesh->createPolygon(points,color);
            model->appendMesh(mesh);
            auto transform = std::make_shared<SceneTransform>();
            transform->addChild(model);
            return transform;
        }

        std::vector<Triangle> createPolygonVector(const std::vector<glm::vec3>& points, const glm::vec3& color)
        {
            return Primitive::generatePolygon(points,color);
        }
        std::vector<Triangle> createModelFromCubeVector(glm::vec3 position, glm::mat4 rotation, glm::vec3 sz, glm::vec3 color)
        {
            auto triangles = Primitive::generateBox(color);
            glm::mat4 matrix = glm::translate(position)*glm::transpose(rotation)*glm::scale(sz);

            Primitive::transformTriangles(matrix, triangles);
            return triangles;
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
            
            //LOG_INFO("Rotation : %s\n", glm::to_string(rotation).c_str());

            //rotation = glm::rotate(glm::mat4(1.0f), 30.0f, glm::vec3(1.0f,1.0f,1.0f));

            glm::mat4 matrix = glm::translate(position)*glm::transpose(rotation)*glm::scale(sz);
            //glm::mat4 matrix = rotation;//*glm::scale(sz);
            transform->setTransformation(matrix);
            transform->addChild(model);
            return transform;
        }

        void addBitmapTexture(bitmap_image& img)
        {
            // Bitmap to JSON
            json texture;

           const unsigned int height = img.height();
           const unsigned int width  = img.width();

           texture["heigth"] = (int) height;
           texture["width"] = (int) width;
           texture["_type"] = "texture";

           json pixels;
           for (std::size_t y = 0; y < height; ++y)
           {
              for (std::size_t x = 0; x < width; ++x)
              {
                 rgb_t colour;

                 img.get_pixel(x, y, colour);
                 json pixel;
                 pixel["r"] = (float) colour.red;
                 pixel["g"] = (float) colour.green;
                 pixel["b"] = (float) colour.blue;
                 pixel["_type"] = "pixel";
                 pixels.push_back(pixel);
              }
           }
           texture["pixels"] = pixels;

           // import JSON string
           auto result = pg.appendSymbol(texture);
           if(result == false)
           {
               LOG_ERROR("Failed to append BMP texture to Procgen\n");
           }
        }
};
