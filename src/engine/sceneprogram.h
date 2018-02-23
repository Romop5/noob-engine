#ifndef _SCENEPROGRAM_H
#define _SCENEPROGRAM_H
#include <GL/glew.h>
#include <engine/scenenode.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utils/logger.h>
#include <vector>
#include <cstdio>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

class ShaderProgram {

  private:
    GLuint shaderProgram;

    GLenum getTypeOfShaderFromName(const std::string name) {
        if (name.find("vertex") != std::string::npos)
            return GL_VERTEX_SHADER;
        if (name.find("fragment") != std::string::npos)
            return GL_FRAGMENT_SHADER;
        LOG_ERROR("Unknown shader type. Name: %s\n", name.c_str());
        return GL_FRAGMENT_SHADER;
    }

    GLuint CompileShader(const std::string file) {
        GLenum shaderType = getTypeOfShaderFromName(file);
        LOG_INFO("For shader %s got type: %d\n", file.c_str(),shaderType);
        GLuint shader = glCreateShader(shaderType);

        FILE* inputFile = fopen(file.c_str(), "r");
        if (inputFile == NULL) {
            LOG_ERROR("Failed to open shader %s\n", file.c_str());
            return 0;
        }
        fseek(inputFile,0, SEEK_END); 
        long lengthOfInputText = ftell(inputFile);
        rewind(inputFile);

        char* source = new char[lengthOfInputText+1];
        source[lengthOfInputText] = 0;
        fread(source, 1, lengthOfInputText, inputFile);
        fclose(inputFile);
        
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        delete[] source;

        GLint Result = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &Result);
        int InfoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> errorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(shader, InfoLogLength, NULL, &errorMessage[0]);
            LOG_ERROR("%s\n", &errorMessage[0]);
            LOG_ERROR("In shader: %s\n", file.c_str());

            return 0;
        }

        return shader;
    }

    template <class... Names> std::vector<GLuint> CompileShaders() {
        return {};
    }

    template <class... Names>
    std::vector<GLuint> CompileShaders(std::string name, Names... args) {
        auto others = CompileShaders(args...);
        others.push_back(CompileShader(name));
        return others;
    }

  public:
    template <class... Names> bool LoadShaders(Names... arg) {
        auto compiledShaders = CompileShaders(arg...);

        shaderProgram = glCreateProgram();
        for (auto &shader : compiledShaders)
            glAttachShader(shaderProgram, shader);

        glBindAttribLocation(shaderProgram, 0, "modelPos");
        glBindAttribLocation(shaderProgram, 2, "modelColor");
        glBindAttribLocation(shaderProgram, 3, "modelNormal");

        glLinkProgram(shaderProgram);

        GLint Result = GL_FALSE;
        glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &Result);

        int InfoLogLength = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> errorMessage(InfoLogLength + 1);
            glGetProgramInfoLog(shaderProgram, InfoLogLength, NULL, &errorMessage[0]);
            LOG_ERROR("Linking: %s\n", &errorMessage[0]);

            return false;
        }


     

        for (auto &shader : compiledShaders) {
            glDetachShader(shaderProgram, shader);
            glDeleteShader(shader);
        }
        return true;
    }

    GLuint getProgramId() const { return shaderProgram; }
};

class SceneShader : public SceneNode {
    std::shared_ptr<ShaderProgram> _program;
    public:
    SceneShader() { _typeOfNode = SceneNodeType::SHADER; }
    SceneShader(std::shared_ptr<ShaderProgram> sp) {
         _typeOfNode = SceneNodeType::SHADER; 
        _program = sp;
        
    }
    virtual json this_json() const { return json("SceneShader"); }

    GLuint getProgram() const { return _program->getProgramId(); }
    void setProgram(std::shared_ptr<ShaderProgram> program) {
        this->_program = program;
    }

    virtual void render(RenderState& state) override
    {
        GLuint oldProgram = state.program;
        state.countOfLights++;
        state.program = this->getProgram(); 
        glUseProgram(state.program);

        // set perspective
        GLint loc = glGetUniformLocation(state.program, "projectionview");
        glUniformMatrix4fv(loc,1,GL_FALSE, glm::value_ptr(state.projectionview));
        // Pre
        for(auto &child: this->getChildren())
        {
            child->render(state);
        } 
        // Revert
        state.countOfLights--;
        state.program = this->getProgram();
        glUseProgram(state.program);
    }

        
};
#endif
