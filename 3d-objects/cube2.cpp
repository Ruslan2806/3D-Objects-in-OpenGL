// #################################################### TASK 3 ####################################################

// Кубик с двумя смешанными текстурами. 
// Пропорция должна регулироваться кнопками клавиатуры.

#include "shader.h"

GLuint VBO_Cube2;
GLuint ProgramCube2;

void Cube2_InitShader() {
    // TODO
}

void Cube2_InitVBO() {
    // TODO
    checkOpenGLerror();
}

void Cube2_Draw() 
{
    // TODO
}

void Cube2_Release() {
    glDeleteBuffers(1, &VBO_Cube2);
    glDeleteProgram(ProgramCube2);
}