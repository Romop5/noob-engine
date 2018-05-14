/**
 * @file ./engine/renderstate.h
 * @brief Class for uniforms and transformation
 * @copyright The MIT license 
 */

#ifndef _RENDER_STATE_H
#define _RENDER_STATE_H

#include <glm/glm.hpp>
#include <GL/glew.h>
struct RenderState
{
    // Model-View-Projection matrix
    glm::mat4 mvp;
    // Projection View matrix
    glm::mat4 projectionview;
    // Shader program
    GLuint program;
    // Count of lights
    GLint  countOfLights;
    

	RenderState(): mvp(1.0), projectionview(1.0), program (0), countOfLights(0) {}
};

#endif
