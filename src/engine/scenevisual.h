#include <engine/mesh.h>
#include <engine/scenenode.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

class SceneVisual : public SceneNode {
    std::vector<Mesh> meshes;

  public:
    SceneVisual() { this->_typeOfNode = SceneNodeType::VISUAL; }
    virtual json this_json() const { return json("VISUAL"); }
    std::vector<Mesh> &getMeshes() { return this->meshes; }
    void appendMesh(const Mesh &mesh) { this->meshes.push_back(mesh); }

    virtual void render(RenderState& state)
    {
        for(auto &mesh: this->meshes)
        {
            //LOG_INFO("Drawing a mesh with transform:\n");
            //LOG_INFO("%s\n", glm::to_string(state.mvp).c_str());
            GLint loc = glGetUniformLocation(state.program, "mvp");
            glUniformMatrix4fv(loc,1,GL_FALSE, glm::value_ptr(state.mvp));
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, mesh.getVertexBufferObjectId());

            glVertexAttribPointer(0, // attribute 0. No particular reason for 0,
                                     // but must match the layout in the shader.
                                  3, // size
                                  GL_FLOAT, // type
                                  GL_FALSE, // normalized?
                                  0,        // stride
                                  (void *)0 // array buffer offset
            );
            // Draw the triangle !
            glDrawArrays(
                GL_TRIANGLES, 0,
                mesh.getVerticesCount()); // Starting from vertex 0; 3 vertices total -> 1 triangle
            glDisableVertexAttribArray(0);  
        }

        // Pre
        for(auto &child: this->getChildren())
        {
            child->render(state);
        } 
        // Revert
    }
        

};
