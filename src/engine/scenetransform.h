#ifndef _SCENETRANSFOR_H
#define _SCENETRANSFOR_H
#include <engine/scenenode.h>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class SceneTransform : public SceneNode 
{   
    protected:
    glm::mat4    _finalTransformation; 
    public:
    SceneTransform() {
        _typeOfNode = SceneNodeType::TRANSFORMATION;
    }
    glm::mat4  getTransformation() {return _finalTransformation;}
};
#endif
