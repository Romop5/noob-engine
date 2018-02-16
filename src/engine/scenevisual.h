#include <engine/mesh.h>
#include <engine/scenenode.h>

class SceneVisual : public SceneNode {
    std::vector<Mesh> meshes;

  public:
    SceneVisual() { this->_typeOfNode = SceneNodeType::VISUAL; }
    virtual json this_json() const { return json("VISUAL"); }
    std::vector<Mesh> &getMeshes() { return this->meshes; }
    void appendMesh(const Mesh &mesh) { this->meshes.push_back(mesh); }
};
