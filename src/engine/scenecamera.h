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

    float _width;
    float _height;
    float _fov;

    public:
    SceneCamera(): _isDirty(true), _width(800.0), _height(600) {}
    virtual json this_json() const { return json("Camera"); }

    virtual void render(RenderState& state)
    {
        if(_isDirty)
            calculateNodeTransform();

        state.projectionview = _perspective*_view;

        SceneTransform::render(state);
    }
    void setPerspectiveTransform(float fovAngle)
    {
        this->_fov = fovAngle;
        LOG_INFO("Transform: perspective  %f\n",fovAngle);
        this->_perspective = glm::perspective(glm::radians(fovAngle), (float) _width / (float) _height, 0.1f, 1000.0f);
        this->_isDirty = true;
    }
    void setViewTransform(glm::mat4 transform)
    {
	LOG_INFO("Transform: view %s\n", glm::to_string(transform).c_str());
        this->_view = transform;
        this->_isDirty = true;
    }

    void resize(float width, float height)
    {
        this->_width = width;
        this->_height = height;
        this->setPerspectiveTransform(this->_fov);
    }
    void calculateNodeTransform()
    {
        //this->setTransformation(this->_view); 
    }

};
#endif
