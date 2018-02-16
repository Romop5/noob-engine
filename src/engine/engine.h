#include "scenenode.h"
#include "scenetransform.h"
#include "scenevisual.h"
#include <SDL2/SDL.h>

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

class Engine {
    std::shared_ptr<SceneNode> _scene;
    SDL_Window *_window;
    SDL_GLContext _mainContext;
    void _render(const glm::mat4 &world, std::shared_ptr<SceneNode> &node);
    bool _isRunning;

  public:
    Engine() : _isRunning(true) {}
    ~Engine();
    bool createWindows(float width, float heigth);
    void resizeWindows(float width, float heigth);
    void setWindowsVisible(bool status);
    void setScene(const std::shared_ptr<SceneNode> &scene);
    const std::shared_ptr<SceneNode> &getScene();
    void render();
    void handleEvents();
    bool isRunning() const { return this->_isRunning; }
};
