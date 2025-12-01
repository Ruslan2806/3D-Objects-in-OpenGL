#include "shader.h"
#include <iostream>
#include <vector>
#include <cmath>

Assignment currentAssignment = ASSIGNMENT_1;

void ShaderLog(unsigned int shader) {
    int infologLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
    if (infologLen > 1) {
        int charsWritten = 0;
        std::vector<char> infoLog(infologLen);
        glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog.data());
        std::cout << "InfoLog: " << infoLog.data() << std::endl;
    }
}

void InitShader() {

}

void InitVBO() {

}

void Init() {
    InitShader();
    InitVBO();
}

void Draw() {

}

void Release() {

}