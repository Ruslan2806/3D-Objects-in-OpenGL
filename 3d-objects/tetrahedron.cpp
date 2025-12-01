#include "tetrahedron.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SFML/Window.hpp>

GLuint VBO_Tetrahedron;
GLuint ProgramTetrahedron;
glm::vec3 tetrahedronPos;

static GLuint Attrib_vertex_tetrahedron;
static GLuint Attrib_color_tetrahedron;

static GLuint Uniform_offset;
static GLuint Uniform_projection;
static GLuint Uniform_view;
static GLuint Uniform_model;

const char* VertexShaderTetrahedron = R"(
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 offset;

out vec4 vertexColor;

void main() {
    vec3 adjustedPosition = position + offset;
    gl_Position = projection * view * model * vec4(adjustedPosition, 1.0);
    vertexColor = color;
}
)";

const char* FragShaderTetrahedron = R"(
#version 330 core

in vec4 vertexColor;
out vec4 FragColor;

void main() {
    FragColor = vertexColor;
}
)";

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

void checkOpenGLerror() {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
}

void Tetrahedron_InitShader() {
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &VertexShaderTetrahedron, NULL);
    glCompileShader(vShader);
    ShaderLog(vShader);

    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &FragShaderTetrahedron, NULL);
    glCompileShader(fShader);
    ShaderLog(fShader);

    ProgramTetrahedron = glCreateProgram();
    glAttachShader(ProgramTetrahedron, vShader);
    glAttachShader(ProgramTetrahedron, fShader);
    glLinkProgram(ProgramTetrahedron);

    Attrib_vertex_tetrahedron = glGetAttribLocation(ProgramTetrahedron, "position");
    if (Attrib_vertex_tetrahedron == (GLuint)-1) {
        std::cout << "Could not bind attrib position" << std::endl;
    }
    
    Attrib_color_tetrahedron = glGetAttribLocation(ProgramTetrahedron, "color");
    if (Attrib_color_tetrahedron == (GLuint)-1) {
        std::cout << "Could not bind attrib color" << std::endl;
    }

    Uniform_offset = glGetUniformLocation(ProgramTetrahedron, "offset");
    Uniform_projection = glGetUniformLocation(ProgramTetrahedron, "projection");
    Uniform_view = glGetUniformLocation(ProgramTetrahedron, "view");
    Uniform_model = glGetUniformLocation(ProgramTetrahedron, "model");

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    checkOpenGLerror();
}

std::vector<Vertex3DWithColor> createTetrahedron() {
    std::vector<Vertex3DWithColor> vertices;

    float h = 0.816f;  // высота тетраэдра
    float r = 0.577f;  // радиус описанной окружности основания
    
    // Верхняя вершина (красная)
    float top_x = 0.0f, top_y = h, top_z = 0.0f;
    
    // Три вершины основания
    float base_y = -0.3f;
    float v1_x = r, v1_y = base_y, v1_z = 0.0f;                    // Зелёная (вправо)
    float v2_x = -r/2.0f, v2_y = base_y, v2_z = r*0.866f;          // Синяя (назад-левая)
    float v3_x = -r/2.0f, v3_y = base_y, v3_z = -r*0.866f;         // Жёлтая (вперед-левая)

    // Грань 1: верх - зелёная - синяя
    vertices.push_back({top_x, top_y, top_z, 1.0f, 0.0f, 0.0f, 1.0f}); // красная
    vertices.push_back({v1_x, v1_y, v1_z, 0.0f, 1.0f, 0.0f, 1.0f}); // зелёная
    vertices.push_back({v2_x, v2_y, v2_z, 0.0f, 0.0f, 1.0f, 1.0f}); // синяя

    // Грань 2: верх - синяя - жёлтая
    vertices.push_back({top_x, top_y, top_z, 1.0f, 0.0f, 0.0f, 1.0f}); // красная
    vertices.push_back({v2_x, v2_y, v2_z, 0.0f, 0.0f, 1.0f, 1.0f}); // синяя
    vertices.push_back({v3_x, v3_y, v3_z, 1.0f, 1.0f, 0.0f, 1.0f}); // жёлтая

    // Грань 3: верх - жёлтая - зелёная
    vertices.push_back({top_x, top_y, top_z, 1.0f, 0.0f, 0.0f, 1.0f}); // красная
    vertices.push_back({v3_x, v3_y, v3_z, 1.0f, 1.0f, 0.0f, 1.0f}); // жёлтая
    vertices.push_back({v1_x, v1_y, v1_z, 0.0f, 1.0f, 0.0f, 1.0f}); // зелёная

    // Грань 4: основание - зелёная - синяя - жёлтая
    vertices.push_back({v1_x, v1_y, v1_z, 0.0f, 1.0f, 0.0f, 1.0f}); // зелёная
    vertices.push_back({v3_x, v3_y, v3_z, 1.0f, 1.0f, 0.0f, 1.0f}); // жёлтая
    vertices.push_back({v2_x, v2_y, v2_z, 0.0f, 0.0f, 1.0f, 1.0f}); // синяя

    return vertices;
}

void Tetrahedron_InitVBO() {
    std::vector<Vertex3DWithColor> tetrahedronVertices = createTetrahedron();

    glGenBuffers(1, &VBO_Tetrahedron);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Tetrahedron);
    glBufferData(GL_ARRAY_BUFFER, 
                 tetrahedronVertices.size() * sizeof(Vertex3DWithColor), 
                 tetrahedronVertices.data(), 
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    checkOpenGLerror();
}

void Tetrahedron_HandleKeyboard() {
    float speed = 0.02f;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        tetrahedronPos.x -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        tetrahedronPos.x += speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        tetrahedronPos.y += speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        tetrahedronPos.y -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        tetrahedronPos.z -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        tetrahedronPos.z += speed;
    }
}

void Tetrahedron_Draw() {
    Tetrahedron_HandleKeyboard();

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Tetrahedron);
    glUseProgram(ProgramTetrahedron);

    glEnableVertexAttribArray(Attrib_vertex_tetrahedron);
    glEnableVertexAttribArray(Attrib_color_tetrahedron);

    glVertexAttribPointer(Attrib_vertex_tetrahedron, 3, GL_FLOAT, GL_FALSE,
                         sizeof(Vertex3DWithColor), (void*)0);
    
    glVertexAttribPointer(Attrib_color_tetrahedron, 4, GL_FLOAT, GL_FALSE,
                         sizeof(Vertex3DWithColor), 
                         (void*)offsetof(Vertex3DWithColor, r));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 2.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f) 
    );

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(Uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(Uniform_view, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(Uniform_model, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(Uniform_offset, 1, glm::value_ptr(tetrahedronPos));

    glDrawArrays(GL_TRIANGLES, 0, 12);

    glDisableVertexAttribArray(Attrib_vertex_tetrahedron);
    glDisableVertexAttribArray(Attrib_color_tetrahedron);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

void Tetrahedron_Release() {
    glDeleteBuffers(1, &VBO_Tetrahedron);
    glDeleteProgram(ProgramTetrahedron);
}