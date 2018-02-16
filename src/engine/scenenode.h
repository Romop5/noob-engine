#ifndef _SCENENODE_H
#define _SCENENODE_H
#include "treetemplate.h"
#include <json.hpp>
#include <memory>

using json = nlohmann::json;

enum class SceneNodeType { UNKNOWN, VISUAL, TRANSFORMATION, CAMERA };
class SceneNode : public TreeNode<std::shared_ptr<SceneNode>> {
  protected:
    SceneNodeType _typeOfNode;

  public:
    SceneNode() : _typeOfNode(SceneNodeType::UNKNOWN) {}
    const SceneNodeType getNodeType() const { return _typeOfNode; }
    virtual json this_json() const { return json("SceneNode"); }
    json to_json() const {
        json childrenjson;
        for (auto &child : this->getChildren())
            childrenjson.push_back(child->to_json());

        if (this->getChildren().size() > 0) {
            json dmp = {{"_type: ", this->this_json()},
                        {"children:", childrenjson}};
            return dmp;
        } else {
            return this->this_json();
        }
    }
};
#endif
