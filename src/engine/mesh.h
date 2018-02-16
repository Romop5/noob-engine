#include "GL.h"

struct Vertex {
    glm::vec3 x, y, z;
}

class Mesh {
    GLuint vao;
    GLuint bufferId;

  public:
    void createFromVertices(std::vector<Vertex> vertices) {}
};
