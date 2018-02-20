#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H
#include <engine/mesh.h>

class Primitive: public Mesh
{
    public:
        void createBox(glm::vec3 color = glm::vec3(0.0))
        {
            std::vector<Triangle> triangles;
            // front 
            triangles.push_back( {
                Vertex(glm::vec3(-1.0f, -1.0f, 1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(1.0f, -1.0f, 1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(color))

            });
            triangles.push_back( {
                 Vertex(glm::vec3(-1.0f, -1.0f, 1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(color))
            });
             //back 
            triangles.push_back( {
                 Vertex(glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(1.0f, -1.0f, -1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(1.0f, 1.0f, -1.0f),glm::vec3(color))
            });
            triangles.push_back( {
                 Vertex(glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(-1.0f, 1.0f, -1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(1.0f, 1.0f, -1.0f),glm::vec3(color))
            });

             // left
            triangles.push_back( {
                 Vertex(glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(-1.0f, 1.0f, -1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec3(color))
            });
            triangles.push_back( {
                 Vertex(glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(-1.0f, -1.0f, 1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec3(color))
            });

            //right 
            triangles.push_back( {
                 Vertex(glm::vec3(1.0f, -1.0f, -1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(1.0f, 1.0f, -1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(color))
            });
            triangles.push_back( {
                 Vertex(glm::vec3(1.0f, -1.0f, -1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(1.0f, -1.0f, 1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(color))
            });
            //bottom 
            triangles.push_back( {
                 Vertex(glm::vec3(1.0f, -1.0f, 1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(1.0f, -1.0f, -1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(-1.0f,-1.0f, 1.0f),glm::vec3(color))
            });

            triangles.push_back( {
                 Vertex(glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(1.0f, -1.0f, -1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(-1.0f, -1.0f, 1.0f),glm::vec3(color))
            });
            // up 
            triangles.push_back( {
                 Vertex(glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(1.0f, 1.0f, -1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec3(color))
            });

            triangles.push_back( {
                 Vertex(glm::vec3(-1.0f, 1.0f, -1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(1.0f, 1.0f, -1.0f),glm::vec3(color)),
                 Vertex(glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec3(color))
            });



            this->createFromVertices(triangles,
                VertexAtributes::POSITION | VertexAtributes::COLOR
                | VertexAtributes::NORMAL);
                //VertexAtributes::POSITION );
         }
};
#endif
