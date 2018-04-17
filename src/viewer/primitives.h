#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H
#include <engine/mesh.h>

class Primitive: public Mesh
{
    public:
        void createPolygon(const std::vector<glm::vec3>& points, const glm::vec3& color)
        {
            const glm::vec3& start = points[0];
            glm::vec3 lastOne = points[1];
            std::vector<Triangle> triangles;

            for(auto it = points.begin()+2; it != points.end(); it++)
            {
                triangles.push_back( {
                    Vertex(start,glm::vec3(color)),
                     Vertex(lastOne,glm::vec3(color)),
                     Vertex(*it,glm::vec3(color))
                });
                lastOne = *it;
            }
            LOG_DEBUG("Adding polygon\n");
            this->createFromVertices(triangles,
            VertexAtributes::POSITION | VertexAtributes::COLOR
            | VertexAtributes::NORMAL);
        }

        void createBox(glm::vec3 color = glm::vec3(0.0))
        {
            LOG_DEBUG("Adding box with color: %s\n", glm::to_string(color).c_str());
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
