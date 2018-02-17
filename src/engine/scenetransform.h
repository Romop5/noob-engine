#ifndef _SCENETRANSFOR_H
#define _SCENETRANSFOR_H
#include <engine/scenenode.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

class SceneTransform : public SceneNode {
  protected:
    glm::mat4 _finalTransformation;

  public:
    SceneTransform() { _typeOfNode = SceneNodeType::TRANSFORMATION; }
    glm::mat4 getTransformation() { return _finalTransformation; }

    virtual json this_json() const override { return json("Transform"); }

    void setTransformation(glm::mat4 newTrans) {
        this->_finalTransformation = newTrans;
    }

    
    virtual void render(RenderState& state)
    {
        // Pre
        glm::mat4 oldTransform = state.mvp;
        state.mvp =  state.mvp*this->getTransformation();
        for(auto &child: this->getChildren())
        {
            child->render(state);
        } 
        // Revert
        state.mvp = oldTransform;
    }



};
#endif
