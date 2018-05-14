/**
 * @file ./engine/scenelight.h
 * @brief Light node
 * @copyright The MIT license 
 */

#ifndef _LIGHT_H
#include <engine/scenenode.h>

class SceneLight: public SceneNode
{
    private:
    glm::vec3 _position;
    glm::vec3 _colour;
    public:
    SceneLight (): _position(0.0,0.0,0.0),_colour(1.0,0.0,0.0)	{ _typeOfNode = SceneNodeType::LIGHT; }
    virtual json this_json() const { return json("Light"); }

    /**
     * @brief Prepare light rendering - change uniforms
     *
     * @param state
     */
    virtual void render(RenderState& state) override
    {
        auto countOfLightsLocation = glGetUniformLocation(
                            state.program, "countOfLights");
        
        state.countOfLights++; 
        glUniform1i(countOfLightsLocation,state.countOfLights);
        // place it to shader
        //
        char lightUniformName[50];
        sprintf(lightUniformName, "lights[%d]", state.countOfLights-1);
        auto lightPlace = glGetUniformLocation(state.program,lightUniformName); 
        glUniform3f(lightPlace, _position[0], _position[1],_position[2]);

        // render children
       for(auto &child: this->getChildren())
            child->render(state); 
        // decrease count
        state.countOfLights--; 
        glUniform1i(countOfLightsLocation,state.countOfLights);
    }
};

#endif
