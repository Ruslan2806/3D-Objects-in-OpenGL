// #################################################### TASK 2 ####################################################

// Кубик с наложенной на него текстурой. 
// Текстура должна быть соединена с цветами, влияние цвета регулируется кнопками клавиатуры.

#include "shader.h"

GLuint VBO_Cube1;
GLuint ProgramCube1;

void Cube1_InitShader() {
    // TODO
}

void Cube1_InitVBO() {
    // TODO
    checkOpenGLerror();
}

void Cube1_Draw() 
{
    // TODO
}

void Cube1_Release() {
    glDeleteBuffers(1, &VBO_Cube1);
    glDeleteProgram(ProgramCube1);
}