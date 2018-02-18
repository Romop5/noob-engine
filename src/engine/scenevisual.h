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
          //  glBindBuffer(GL_ARRAY_BUFFER, mesh.getVertexBufferObjectId());
                       // Draw the triangle !

            glBindVertexArray(mesh.getVertexArrayObjectId());

            glDrawArrays(GL_TRIANGLES,0, mesh.getVerticesCount());

            glBindVertexArray(0);
            //glDisableVertexAttribArray(0);  
        }

        // Pre
        for(auto &child: this->getChildren())
        {
            child->render(state);
        } 
        // Revert
    }
        

};
