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
