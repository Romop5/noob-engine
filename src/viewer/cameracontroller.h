#ifndef _CAMERACONTROLLER_H
#define _CAMERACONTROLLER_H
#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <engine/scenecamera.h>

class CameraController
{
   private:
    std::shared_ptr<SceneCamera> _cameraNode; 
    glm::vec3    _position;
    glm::mat4   _rotationMatrix;
    glm::vec2   _angles;
    bool _isDirty;

    void moveCameraBy(glm::vec3 _offset)
    {
        glm::vec4 towards = glm::inverse(_rotationMatrix)*glm::vec4(_offset,1.0);
        //LOG_INFO("Towards: %s\n", glm::to_string(towards).c_str());
        this->_position += glm::vec3(towards);
        this->_isDirty = true;
    }
    void rotateCameraBy(glm::vec2 _offset)
    {
        // Add up current angle with difference
        this->_angles += _offset;

        auto horizontalRotation = glm::rotate(this->_angles.x, glm::vec3(0.0,1.0,0.0));
        this->_rotationMatrix = 
                    glm::rotate(this->_angles.y, glm::vec3(1.0,0.0,0.0))*
                    horizontalRotation;
        this->_isDirty = true;
    }
    void update()
    {
        if(this->_cameraNode == nullptr)
            return;
        if(_isDirty)
            calculateTransform();
    }

    void calculateTransform()
    {
         this->_cameraNode->setViewTransform(
                    glm::translate(_rotationMatrix, _position));
        this->_isDirty = false;
    }
    
    public:
    CameraController(): _isDirty(true) {}
    CameraController(std::shared_ptr<SceneCamera> cam): _isDirty(true),
            _cameraNode(cam) {}

    void setCameraNode(std::shared_ptr<SceneCamera> cam)
    {
        this->_cameraNode = cam;
    }
    
    void processMessage(SDL_Event event)
    {  
        switch(event.type)
        {
            case SDL_KEYDOWN:
            {
                LOG_INFO("Here we are - and event\n");
                if(event.key.keysym.sym == SDLK_a)
                    this->moveCameraBy(glm::vec3(1.0,0.0,0.0)); 

                if(event.key.keysym.sym == SDLK_d)
                    this->moveCameraBy(glm::vec3(-1.0,0.0,0.0)); 

                if(event.key.keysym.sym == SDLK_w)
                    this->moveCameraBy(glm::vec3(0.0,0.0,1.0)); 

                if(event.key.keysym.sym == SDLK_s)
                    this->moveCameraBy(glm::vec3(0.0,0.0,-1.0)); 

                if(event.key.keysym.sym == SDLK_UP)
                    this->rotateCameraBy(glm::vec2(0.0,-0.1));

                if(event.key.keysym.sym == SDLK_DOWN)
                    this->rotateCameraBy(glm::vec2(0.0,0.1));

                if(event.key.keysym.sym == SDLK_LEFT)
                    this->rotateCameraBy(glm::vec2(-0.1,0.0));

                if(event.key.keysym.sym == SDLK_RIGHT)
                    this->rotateCameraBy(glm::vec2(0.1,0.0));
            }
        }
        // Update transform
        update();
    }
};
#endif
