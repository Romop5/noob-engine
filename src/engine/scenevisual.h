#include <engine/scenenode.hpp>

class SceneVisual : public SceneNode {
    std::vector<Mesh> meshes;

  public:
    SceneVisual() { this->_typeOfNode = SceneNodeType::VISUAL; }
    virtual json this_json() const { return json("VISUAL"); }
    std::vector<Mesh> &getMeshes() const { return this->meshes; }
};
