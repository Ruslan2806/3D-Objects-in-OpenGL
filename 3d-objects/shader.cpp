#include "shader.h"
#include "tetrahedron.h"
#include <GL/glew.h>

Assignment currentAssignment = ASSIGNMENT_1;

void Init() {
    switch (currentAssignment) {
        case ASSIGNMENT_1:
            Tetrahedron_InitShader();
            Tetrahedron_InitVBO();
            break;
        case ASSIGNMENT_2:
            // TODO: 
            break;
        case ASSIGNMENT_3:
            // TODO: 
            break;
        case ASSIGNMENT_4:
            // TODO: 
            break;
    }
    glEnable(GL_DEPTH_TEST);
}

void Draw() {
    switch (currentAssignment) {
        case ASSIGNMENT_1:
            Tetrahedron_Draw();
            break;
        case ASSIGNMENT_2:
            // TODO: 
            break;
        case ASSIGNMENT_3:
            // TODO: 
            break;
        case ASSIGNMENT_4:
            // TODO: 
            break;
    }
}

void Release() {
    switch (currentAssignment) {
        case ASSIGNMENT_1:
            Tetrahedron_Release();
            break;
        case ASSIGNMENT_2:
            // TODO: 
            break;
        case ASSIGNMENT_3:
            // TODO: 
            break;
        case ASSIGNMENT_4:
            // TODO: 
            break;
    }
}
