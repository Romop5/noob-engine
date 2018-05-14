/**
 * @file ./engine/scenevisual.h
 * @brief Mesh node
 * @copyright The MIT license 
 */

#ifndef _SCENEVISUAL_H
#define _SCENEVISUAL_H

#include <engine/mesh.h>
#include <engine/scenenode.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <memory>

class SceneVisual : public SceneNode {
    std::vector<std::shared_ptr<Mesh>> meshes;

  public:
    SceneVisual() { this->_typeOfNode = SceneNodeType::VISUAL; }
    virtual json this_json() const { return json("VISUAL"); }
    std::vector<std::shared_ptr<Mesh>> &getMeshes() { return this->meshes; }
    void appendMesh(const std::shared_ptr<Mesh> &mesh) { this->meshes.push_back(mesh); }

    virtual void render(RenderState& state)
    {
        for(auto &mesh: this->meshes)
        {
            // Render mesh
            GLint loc = glGetUniformLocation(state.program, "world");
            glUniformMatrix4fv(loc,1,GL_FALSE, glm::value_ptr(state.mvp));
            glBindVertexArray(mesh->getVertexArrayObjectId());
            glDrawArrays(GL_TRIANGLES,0, mesh->getVerticesCount());
            glBindVertexArray(0);
        }

        // Pre
        for(auto &child: this->getChildren())
        {
            child->render(state);
        } 
        // Revert
    }
        

};
#endif
