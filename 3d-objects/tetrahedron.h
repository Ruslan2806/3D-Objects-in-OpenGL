#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "shader.h"

extern GLuint VBO_Tetrahedron;
extern GLuint ProgramTetrahedron;
extern glm::vec3 tetrahedronPos;

void Tetrahedron_InitShader();
void Tetrahedron_InitVBO();
void Tetrahedron_HandleKeyboard();
void Tetrahedron_Draw();
void Tetrahedron_Release();

std::vector<Vertex3DWithColor> createTetrahedron();
