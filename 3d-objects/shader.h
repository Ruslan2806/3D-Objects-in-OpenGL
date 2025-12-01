#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

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