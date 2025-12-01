#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SFML/Window.hpp>

enum Assignment {
    ASSIGNMENT_1 = 1,
    ASSIGNMENT_2 = 2,
    ASSIGNMENT_3 = 3,
    ASSIGNMENT_4 = 4  
};

extern Assignment currentAssignment;

struct Vertex3D {
    GLfloat x, y, z;
};

struct Vertex3DWithColor {
    GLfloat x, y, z;
    GLfloat r, g, b, a;
};

struct Vertex3DWithTex {
    GLfloat x, y, z;
    GLfloat u, v;
};

void ShaderLog(unsigned int shader);
void checkOpenGLerror();
void Init();
void Draw();
void Release();

// TASK 1
extern GLuint VBO_Tetrahedron;
extern GLuint ProgramTetrahedron;
extern glm::vec3 tetrahedronPos;

void Tetrahedron_InitShader();
void Tetrahedron_InitVBO();
void Tetrahedron_HandleKeyboard();
void Tetrahedron_Draw();
void Tetrahedron_Release();

std::vector<Vertex3DWithColor> createTetrahedron();

// TASK 2
extern GLuint VBO_Circle;
extern GLuint ProgramCircle;

void Circle_InitShader();
void Circle_InitVBO();
void Circle_Draw();
void Circle_Release();

// TASK 3
extern GLuint VBO_Cube1;
extern GLuint ProgramCube1;

void Cube1_InitShader();
void Cube1_InitVBO();
void Cube1_Draw();
void Cube1_Release();

// TASK 4
extern GLuint VBO_Cube2;
extern GLuint ProgramCube2;

void Cube2_InitShader();
void Cube2_InitVBO();
void Cube2_Draw();
void Cube2_Release();
