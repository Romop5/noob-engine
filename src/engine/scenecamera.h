#ifndef _SCENECAMERA_H
#define _SCENECAMERA_H
#include <engine/scenetransform.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

class SceneCamera : public SceneTransform 
{
    // SceneTransform invalidation flag
    bool _isDirty;
    glm::mat4 _perspective;
    glm::mat4 _view;
    public:
    SceneCamera(): _isDirty(true) {}
    virtual json this_json() const { return json("Camera"); }

    virtual void render(RenderState& state)
    {
        if(_isDirty)
            calculateNodeTransform();

        state.projectionview = _perspective*_view;

        SceneTransform::render(state);
    }
    void setPerspectiveTransform(glm::mat4 transform)
    {
        this->_perspective = transform;
        this->_isDirty = true;
    }
    void setViewTransform(glm::mat4 transform)
    {
        this->_view = transform;
        this->_isDirty = true;
    }

    void calculateNodeTransform()
    {
        //this->setTransformation(this->_view); 
    }

};
#endif
