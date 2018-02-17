#ifndef _SCENECAMERA_H
#define _SCENECAMERA_H
#include <engine/scenetransform.h>

class SceneCamera : public SceneTransform 
{
    // SceneTransform invalidation flag
    bool _isDirty;
    glm::mat4 _perspective;
    glm::mat4 _world;
    public:
    SceneCamera(): _isDirty(true) {}
    virtual json this_json() const { return json("Camera"); }

    virtual void render(RenderState& state)
    {
        if(_isDirty)
            calculateNodeTransform();

        SceneTransform::render(state);
    }
    void setPerspectiveTransform(glm::mat4 transform)
    {
        this->_perspective = transform;
        this->_isDirty = true;
    }
    void setWorldTransform(glm::mat4 transform)
    {
        this->_world = transform;
        this->_isDirty = true;
    }

    void calculateNodeTransform()
    {
        this->setTransformation(this->_perspective*this->_world); 
    }

};
#endif
