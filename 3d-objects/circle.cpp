// #################################################### TASK 4 ####################################################

// Градиентный круг (окружность проходит Hue из HSV, центр белый), 
// который можно масштабировать по осям (каждую ось своей кнопкой).

#include "shader.h"

GLuint VBO_Circle;
GLuint ProgramCircle;

void Circle_InitShader() {
    // TODO
}

void Circle_InitVBO() {
    // TODO
    checkOpenGLerror();
}

void Circle_Draw() 
{
    // TODO
}

void Circle_Release() {
    glDeleteBuffers(1, &VBO_Circle);
    glDeleteProgram(ProgramCircle);
}