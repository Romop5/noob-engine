#ifndef _RENDER_STATE_H
#define _RENDER_STATE_H

#include <glm/glm.hpp>
#include <GL/glew.h>
struct RenderState
{
    glm::mat4 mvp;
    glm::mat4 projection;
    GLuint program;
    GLint  countOfLights;
};

#endif
