#include "scenenode.h"
#include "scenetransform.h"

class Engine {
    std::shared_ptr<SceneNode> scene;

  private:
    void _render(const glm::mat4 &world, std::shared_ptr<SceneNode> &node);

  public:
    void createWindows(float width, float heigth);
    void resizeWindows(float width, float heigth);
    void setWindowsVisible(bool status);
    void setScene(const std::shared_ptr<SceneNode> &scene);
    const std::shared_ptr<SceneNode> &getScene();
    void render();
};
