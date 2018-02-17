#include <engine/mesh.h>

class Primitive: public Mesh
{
    public:
        void createBox()
        {
            std::vector<Triangle> triangles;
            // front 
            triangles.push_back( {
                 glm::vec3(-1.0f, -1.0f, 1.0f),
                 glm::vec3(1.0f, -1.0f, 1.0f),
                 glm::vec3(1.0f, 1.0f, 1.0f)
            });
            triangles.push_back( {
                 glm::vec3(-1.0f, -1.0f, 1.0f),
                 glm::vec3(-1.0f, 1.0f, 1.0f),
                 glm::vec3(1.0f, 1.0f, 1.0f)
            });
             //back 
            triangles.push_back( {
                 glm::vec3(-1.0f, -1.0f, -1.0f),
                 glm::vec3(1.0f, -1.0f, -1.0f),
                 glm::vec3(1.0f, 1.0f, -1.0f)
            });
            triangles.push_back( {
                 glm::vec3(-1.0f, -1.0f, -1.0f),
                 glm::vec3(-1.0f, 1.0f, -1.0f),
                 glm::vec3(1.0f, 1.0f, -1.0f)
            });

             // left
            triangles.push_back( {
                 glm::vec3(-1.0f, -1.0f, -1.0f),
                 glm::vec3(-1.0f, 1.0f, -1.0f),
                 glm::vec3(-1.0f, 1.0f, 1.0f)
            });
            triangles.push_back( {
                 glm::vec3(-1.0f, -1.0f, -1.0f),
                 glm::vec3(-1.0f, -1.0f, 1.0f),
                 glm::vec3(-1.0f, 1.0f, 1.0f)
            });

            //right 
            triangles.push_back( {
                 glm::vec3(1.0f, -1.0f, -1.0f),
                 glm::vec3(1.0f, 1.0f, -1.0f),
                 glm::vec3(1.0f, 1.0f, 1.0f)
            });
            triangles.push_back( {
                 glm::vec3(1.0f, -1.0f, -1.0f),
                 glm::vec3(1.0f, -1.0f, 1.0f),
                 glm::vec3(1.0f, 1.0f, 1.0f)
            });
            // bottom
            triangles.push_back( {
                 glm::vec3(1.0f, -1.0f, 1.0f),
                 glm::vec3(1.0f, -1.0f, -1.0f),
                 glm::vec3(1.0f, -1.0f, 1.0f)
            });
            triangles.push_back( {
                 glm::vec3(-1.0f, -1.0f, -1.0f),
                 glm::vec3(1.0f, -1.0f, -1.0f),
                 glm::vec3(1.0f, -1.0f, 1.0f)
            });
            // up 
            triangles.push_back( {
                 glm::vec3(1.0f, 1.0f, 1.0f),
                 glm::vec3(1.0f, 1.0f, -1.0f),
                 glm::vec3(1.0f, 1.0f, 1.0f)
            });
            triangles.push_back( {
                 glm::vec3(-1.0f, 1.0f, -1.0f),
                 glm::vec3(1.0f, 1.0f, -1.0f),
                 glm::vec3(1.0f, 1.0f, 1.0f)
            });



            this->createFromVertices(triangles);
         }
};
