/**
 * @file ./engine/mesh.h
 * @brief Utility for mesh creation, normal calculation etc
 * @copyright The MIT license 
 */

#ifndef _MESH_H
#define _MESH_H

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

enum VertexAtributes { POSITION = 1, UV=2, COLOR=4, NORMAL=8};

#define COPY_ATTRIBUTE(attr, length, structMember)\
        if(flags & VertexAtributes::attr)\
        {\
            for(int i = 0; i < length; i++)\
                raw[offset+i] = structMember[i];\
            offset += length;\
        }


#define GET_ATTRIBUTE(attr, length, structMember)\
        if(flags & VertexAtributes::attr)\
        {\
            for(int i = 0; i < length; i++)\
                structMember[i] = raw[offset+i];\
            offset += length;\
        }


/**
 * @brief Vertex of polygon
 */
struct Vertex
{
    glm::vec3 _position;
    glm::vec2 _uvCoordinates;
    glm::vec3 _color;
    glm::vec3 _normal;
    Vertex(glm::vec3 pos): _position(pos) {} 
    Vertex(glm::vec3 pos,glm::vec3 color): _position(pos), _color(color) {} 

    /**
     * @brief Serialize vertex to OpenGL buffer 
     *
     * @param raw
     * @param flags
     *
     * @return 
     */
    size_t copyToRawArray(float* raw, size_t flags)
    {
        size_t offset = 0;
        COPY_ATTRIBUTE(POSITION,3, _position);
        COPY_ATTRIBUTE(UV,2, _uvCoordinates);
        COPY_ATTRIBUTE(COLOR,3, _color);
        COPY_ATTRIBUTE(NORMAL,3, _normal);
        return offset;
    }

    /**
     * @brief Deserialize vertex from OpenGL buffer to structure
     *
     * @param raw
     * @param flags
     *
     * @return 
     */
    size_t readFromRawArray(float* raw,size_t flags)
    {
        size_t offset = 0;
        GET_ATTRIBUTE(POSITION,3, _position);
        GET_ATTRIBUTE(UV,2, _uvCoordinates);
        GET_ATTRIBUTE(COLOR,3, _color);
        GET_ATTRIBUTE(NORMAL,3, _normal);
        return offset;
    }
};


struct Triangle{
    Vertex vertices[3];
    void calculateNormals()
    {
        glm::vec3 a = vertices[0]._position-vertices[2]._position;
        glm::vec3 b = vertices[1]._position-vertices[2]._position;
        glm::vec3 normal = glm::normalize(glm::cross(a,b));
        vertices[0]._normal = normal;
        vertices[1]._normal = normal;
        vertices[2]._normal = normal;
    }
};

class Mesh {
    GLuint vao;
    GLuint vbo;
    size_t  verticesCount;

    size_t getSizeOfVertex(size_t flags)
    {
        size_t result = 0;
        if(flags & VertexAtributes::POSITION) result += 3;
        if(flags & VertexAtributes::UV) result += 2;
        if(flags & VertexAtributes::COLOR) result += 3;
        if(flags & VertexAtributes::NORMAL) result += 3;
        return result;
    }
    #define ENABLE_VERTEX_ATTRIBUTE(attr,SIZE,location)\
        if(flags & VertexAtributes::attr)\
        {\
            LOG_DEBUG("Enabling attribute %s of size %d at loc: %d\n", #attr,SIZE,location);\
            glEnableVertexAttribArray(location);\
            glVertexAttribPointer(location, \
                                  SIZE, \
                                  GL_FLOAT, \
                                  GL_FALSE, \
                                  stride, \
                                  (void *)(offset*sizeof(float))\
            );\
            offset+=SIZE;\
        }
    void enableVertexAtributes(size_t flags )
    {

        size_t stride = this->getSizeOfVertex(flags)*sizeof(float);
        LOG_DEBUG("Size of stride: %d\n", stride);
        size_t offset = 0;
        ENABLE_VERTEX_ATTRIBUTE(POSITION, 3, 0);
        ENABLE_VERTEX_ATTRIBUTE(UV, 2, 1);
        ENABLE_VERTEX_ATTRIBUTE(COLOR, 3, 2);
        ENABLE_VERTEX_ATTRIBUTE(NORMAL, 3, 3);
    }
  public:
    Mesh(): verticesCount(0), vao(0), vbo(0)	{ glGenVertexArrays(1, &vao); }
    ~Mesh() { glDeleteVertexArrays(1, &vao); }

    /**
     * @brief Create OpenGL VAO from vector of polygons
     *
     * @param polygons
     * @param flags
     */
    void createFromVertices(std::vector<Triangle> polygons,size_t flags) {
        size_t vertexSize = this->getSizeOfVertex(flags);
        size_t size =  vertexSize *3* polygons.size();

        auto rawArray = new float[size];
        size_t offset = 0;
        // for each polygon
        for (size_t i = 0; i < polygons.size(); i++) {
            // for each vertex
            for(size_t v= 0; v< 3; v++)
            {
                auto polygon = polygons[i];
                // Calculate normal for given vertex
                if(flags & VertexAtributes::NORMAL)
                    polygon.calculateNormals();
                // for each vertex, serialize to OpenGL buffer
                offset += polygon.vertices[v].copyToRawArray(&rawArray[offset],flags);
            }
        }

        LOG_DEBUG("Content of raw buffer:\n");
        for(size_t i = 0; i < size ;i++)
        {
            LOG_DEBUG("%d: %f\n",i,rawArray[i]);
        }

        // Create VAO object
        glGenVertexArrays(1,&vao);
        glBindVertexArray(vao);

        // create vertex buffer
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // copy buffer data to OpenGL memory
        glBufferData(GL_ARRAY_BUFFER, size*sizeof(float), rawArray, GL_STATIC_DRAW);
        delete[] rawArray;

        this->enableVertexAtributes(flags);
        glBindVertexArray(0);
        this->verticesCount = polygons.size()*3;
    }

    /**
     * @brief Reconstruct polygons from OpenGL memory
     *
     * @param polygons
     * @param flags
     */
    void createVerticesFromRaw(std::vector<Triangle>& polygons,size_t flags) {
        size_t vertexSize = this->getSizeOfVertex(flags);
        size_t size =  vertexSize *3* polygons.size();

        auto rawArray = new float[size];
        size_t offset = 0;
        glBindVertexArray(vao);
        float *data = (float *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

        for (size_t i = 0; i < this->verticesCount/3; i++) {
            for(size_t v= 0; v< 3; v++)
            {
                Triangle triangle = {Vertex(glm::vec3(0.0,0.0,0.0)), Vertex(glm::vec3(0.0,0.0,0.0)), Vertex(glm::vec3(0.0,0.0,0.0))};
                offset += triangle.vertices[v].readFromRawArray(&rawArray[offset],flags);
                polygons.push_back(triangle);
            }
        }
        
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindVertexArray(0);
    }


    GLuint getVertexBufferObjectId() const { return this->vbo; }
    GLuint getVertexArrayObjectId() const { return this->vao; }
    size_t getVerticesCount() const { return this->verticesCount; }
};
#endif
