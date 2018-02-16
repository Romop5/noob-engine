#define GL3_PROTOTYPES 1
#include <GL/glew.h>

struct Vertex {
    glm::vec3 a, b, c;
};

class Mesh {
    GLuint vao;
    GLuint vbo;

  public:
    Mesh() { glGenVertexArrays(1, &vao); }
    ~Mesh() { glDeleteVertexArrays(1, &vao); }
    void createFromVertices(std::vector<Vertex> vertices) {
        size_t size = sizeof(float) * 9 * vertices.size();
        auto rawArray = new float[size];
        for (size_t i = 0; i < vertices.size(); i++) {
            rawArray[9 * i + 0] = vertices[i].a.x;
            rawArray[9 * i + 1] = vertices[i].a.y;
            rawArray[9 * i + 2] = vertices[i].a.z;

            rawArray[9 * i + 3] = vertices[i].b.x;
            rawArray[9 * i + 4] = vertices[i].b.y;
            rawArray[9 * i + 5] = vertices[i].b.z;

            rawArray[9 * i + 6] = vertices[i].c.x;
            rawArray[9 * i + 7] = vertices[i].c.y;
            rawArray[9 * i + 8] = vertices[i].c.z;
        }
        glBindVertexArray(vao);

        // create vertex buffer
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, size, rawArray, GL_STATIC_DRAW);
        delete[] rawArray;
    }
    GLuint getVertexBufferObjectId() const { return this->vbo; }
    GLuint getVertexArrayObjectId() const { return this->vao; }
};
