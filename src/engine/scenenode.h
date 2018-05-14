/**
 * @file ./engine/scenenode.h
 * @brief Abstract class for scene nodes
 * @copyright The MIT license 
 */

#ifndef _SCENENODE_H
#define _SCENENODE_H
#include <engine/treetemplate.h>
#include <engine/renderstate.h>
#include <json.hpp>
#include <memory>

using json = nlohmann::json;

/**
 * @brief Types of scene node (transformation, mesh, camera, etc.)
 */
enum class SceneNodeType { UNKNOWN, VISUAL, TRANSFORMATION, CAMERA, SHADER,LIGHT };
class SceneNode : public TreeNode<std::shared_ptr<SceneNode>> {
  protected:
    SceneNodeType _typeOfNode;

  public:
    SceneNode() : _typeOfNode(SceneNodeType::UNKNOWN) {}
    const SceneNodeType getNodeType() const { return _typeOfNode; }
    virtual json this_json() const { return json("SceneNode"); }
    /**
     * @brief Serialize scene node data to JSON
     */
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

    /**
     * @brief Abstract render
     *
     * @param state
     */
    virtual void render(RenderState& state)
    {
        // Pre
        for(auto &child: this->getChildren())
        {
            child->render(state);
        } 
        // Revert
    }
        
};
#endif
