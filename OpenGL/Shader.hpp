#pragma once
#include <GL/glew.h>
#include <string>
#include <iostream>
#include "../renderLib/Matrix.hpp"

#include "File_Utils.hpp"

namespace ES{   

    class Shader{
        GLuint programID;

    public:
        Shader(const std::string& vertexPath, const std::string& fragmentPath) {

            std::string vertexSource = loadFile(vertexPath);
            std::string fragmentSource = loadFile(fragmentPath);

            GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

            const char* v_src = vertexSource.c_str();
            glShaderSource(vertexShader, 1, &v_src, nullptr);
            glCompileShader(vertexShader);

            // --- Vertex Shader Compilation Check ---
            GLint success;
            GLchar infoLog[512];
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
                std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\nPath: " << vertexPath << "\n" << infoLog << std::endl;
            }

            GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

            const char* f_src = fragmentSource.c_str();
            glShaderSource(fragmentShader, 1, &f_src, nullptr);
            glCompileShader(fragmentShader);

            // --- Fragment Shader Compilation Check ---
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
                std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\nPath: " << fragmentPath << "\n" << infoLog << std::endl;
            }

            programID = glCreateProgram();
            glAttachShader(programID, vertexShader);
            glAttachShader(programID, fragmentShader);

            glLinkProgram(programID);

            // --- Shader Program Linking Check ---
            glGetProgramiv(programID, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(programID, 512, NULL, infoLog);
                std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            }

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }

        void use() const {
            glUseProgram(programID);
        }

        GLuint id() const {
            return programID;
        }

        ~Shader() {
            glDeleteProgram(programID);
        }

        void SetMat4(const std::string& name, const Matrix<float,4>& mat) const{
            GLint location = glGetUniformLocation(programID, name.c_str());

            glUniformMatrix4fv(location,1,GL_FALSE, mat.ptr());
        }

        void SetMat3(const std::string& name, const Matrix<float,3>& mat) const{
            GLint location = glGetUniformLocation(programID, name.c_str());

            glUniformMatrix3fv(location,1,GL_FALSE, mat.ptr());
        }
        void SetVec3(const std::string& name, float x, float y, float z) const {
            GLint location = glGetUniformLocation(programID, name.c_str());
            glUniform3f(location, x, y, z);
        }

        void SetInt(const std::string& name, int value) const {
            GLint location = glGetUniformLocation(programID, name.c_str());
            glUniform1i(location, value);
        }

        void SetBool(const std::string& name, bool value) const {
            GLint location = glGetUniformLocation(programID, name.c_str());
            glUniform1i(location, static_cast<int>(value));
        }
        
        void SetFloat(const std::string& name, float value) const {
            GLint location = glGetUniformLocation(programID, name.c_str());
            glUniform1f(location, value);
        }
        
        void SetFloatArray(const std::string& name, const std::vector<float>& values) const {
            if (values.empty()) {
                return; 
            }
            GLint location = glGetUniformLocation(programID, name.c_str());
        
            glUniform1fv(location, values.size(), values.data());
        }

    };

}