#ifndef _ENGINE_H
#define _ENGINE_H

#include <engine/scenenode.h>
#include <engine/sceneprogram.h>
#include <engine/scenecamera.h>
#include <engine/scenetransform.h>
#include <engine/scenevisual.h>
#include <SDL2/SDL.h>

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

class Engine {
    std::shared_ptr<SceneNode> _scene;
    SDL_Window *_window;
    SDL_GLContext _mainContext;
    bool _isRunning;
    std::function<void(SDL_Event)>  _messageCallback;
  public:
    Engine() : _isRunning(true), _window(NULL), _scene(nullptr),_messageCallback(nullptr) {}
    ~Engine();
    bool createWindows(float width, float heigth);
    void resizeWindows(float width, float heigth);
    void setWindowsVisible(bool status);
    void setScene(const std::shared_ptr<SceneNode> &scene);
    std::shared_ptr<SceneNode> getScene() { return _scene; }
    void render();
    void handleEvents();
    bool isRunning() const { return this->_isRunning; }
    void setMessageCallback(std::function<void(SDL_Event)> callback);
};
#endif
