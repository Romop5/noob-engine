#include <GL/glew.h>
#include <engine/scenenode.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utils/logger.h>
#include <vector>

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
        GLuint shader = glCreateShader(shaderType);

        std::ifstream inputFile(file, std::ios::in);
        if (!inputFile.is_open()) {
            LOG_ERROR("Failed to open shader %s\n", file.c_str());
            return 0;
        }
        // Read file
        std::stringstream str;
        str << inputFile.rdbuf();
        // Compile buffer
        const char *inputShade = str.str().c_str();

        glShaderSource(shader, 1, &inputShade, NULL);
        glCompileShader(shader);

        GLint Result = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &Result);
        int InfoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if (InfoLogLength > 0) {
            std::vector<char> errorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(shader, InfoLogLength, NULL, &errorMessage[0]);
            LOG_ERROR("%s\n", &errorMessage[0]);

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
        glLinkProgram(shaderProgram);

        GLint Result = GL_FALSE;
        glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &Result);
        if (Result == GL_FALSE) {
            LOG_ERROR("Failed to link shaders\n");
            return false;
        }

        for (auto &shader : compiledShaders) {
            glDetachShader(shaderProgram, shader);
            glDeleteShader(shader);
        }
        return true;
    }

    GLuint getProgram() const { return shaderProgram; }
};

class SceneShader : public SceneNode {
    SceneShader() { _typeOfNode = SceneNodeType::SHADER; }
    std::shared_ptr<ShaderProgram> program;
    GLuint getProgram() const { return program->getProgram(); }
    void setProgram(std::shared_ptr<ShaderProgram> program) {
        this->program = program;
    }
};
