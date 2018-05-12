#ifndef _RENDER_STATE_H
#define _RENDER_STATE_H

#include <glm/glm.hpp>
#include <GL/glew.h>
struct RenderState
{
    glm::mat4 mvp;
    glm::mat4 projectionview;
    GLuint program;
    GLint  countOfLights;
	RenderState(): mvp(1.0), projectionview(1.0), program (0), countOfLights(0) {}
};

#endif
