// #################################################### TASK 4 ####################################################

// Градиентный круг (окружность проходит Hue из HSV, центр белый),
// который можно масштабировать по осям (каждую ось своей кнопкой).

#include "shader.h"

GLuint VBOCircle;
GLuint ProgramCircle;

glm::vec2 circleScale = glm::vec2(1.0f, 1.0f);  

static GLuint Attrib_vertex_circle;
static GLuint Attrib_color_circle;
static GLuint Uniform_scale;

const char* VertexShaderCircle = R"(
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

uniform vec2 scale;

out vec4 vertexColor;

void main() {
    vec3 scaledPosition = position;
    scaledPosition.x *= scale.x;
    scaledPosition.y *= scale.y;
    gl_Position = vec4(scaledPosition, 1.0);
    vertexColor = color;
}
)";

const char* FragShaderCircle = R"(
#version 330 core

in vec4 vertexColor;

out vec4 FragColor;

void main() {
    FragColor = vertexColor;
}
)";

void Circle_InitShader() {
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &VertexShaderCircle, NULL);
    glCompileShader(vShader);
    ShaderLog(vShader);

    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &FragShaderCircle, NULL);
    glCompileShader(fShader);
    ShaderLog(fShader);

    ProgramCircle = glCreateProgram();
    glAttachShader(ProgramCircle, vShader);
    glAttachShader(ProgramCircle, fShader);
    glLinkProgram(ProgramCircle);

    Attrib_vertex_circle = glGetAttribLocation(ProgramCircle, "position");
    if (Attrib_vertex_circle == (GLuint)-1) {
        std::cout << "Could not bind attrib position" << std::endl;
        return;
    }

    Attrib_color_circle = glGetAttribLocation(ProgramCircle, "color");
    if (Attrib_color_circle == (GLuint)-1) {
        std::cout << "Could not bind attrib color" << std::endl;
        return;
    }

    Uniform_scale = glGetUniformLocation(ProgramCircle, "scale");
    if (Uniform_scale == (GLuint)-1) {
        std::cout << "Could not bind uniform scale" << std::endl;
        return;
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    checkOpenGLerror();
}

glm::vec3 HSVtoRGB(float hue, float saturation, float value) {
    float c = value * saturation;
    float hh = hue / 60.0f;
    float x = c * (1.0f - std::abs(std::fmod(hh, 2.0f) - 1.0f));
    float r = 0.0f, g = 0.0f, b = 0.0f;

    if (hh >= 0 && hh < 1) { r = c; g = x; b = 0; }
    else if (hh >= 1 && hh < 2) { r = x; g = c; b = 0; }
    else if (hh >= 2 && hh < 3) { r = 0; g = c; b = x; }
    else if (hh >= 3 && hh < 4) { r = 0; g = x; b = c; }
    else if (hh >= 4 && hh < 5) { r = x; g = 0; b = c; }
    else if (hh >= 5 && hh <= 6) { r = c; g = 0; b = x; }

    float m = value - c;
    return glm::vec3(r + m, g + m, b + m);
}

std::vector<Vertex3DWithColor> createGradientCircle() {
    std::vector<Vertex3DWithColor> vertices;
    int segments = 60;
    float radius = 0.5f;

    // Центр круга 
    vertices.push_back({0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f});

    // Периметр круга (градиент по Hue)
    for (int i = 0; i <= segments; i++) {
        float angle = (i / (float)segments) * 2.0f * 3.14159265f;
        float x = radius * std::cos(angle);
        float y = radius * std::sin(angle);

        float hue = (i / (float)segments) * 360.0f;
        glm::vec3 color = HSVtoRGB(hue, 1.0f, 1.0f); 

        vertices.push_back({x, y, 0.0f, color.r, color.g, color.b, 1.0f});
    }

    return vertices;
}

void Circle_InitVBO() {
    std::vector<Vertex3DWithColor> circleVertices = createGradientCircle();

    glGenBuffers(1, &VBOCircle);
    glBindBuffer(GL_ARRAY_BUFFER, VBOCircle);
    glBufferData(GL_ARRAY_BUFFER,
                 circleVertices.size() * sizeof(Vertex3DWithColor),
                 circleVertices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    circleScale = glm::vec2(1.0f, 1.0f);

    checkOpenGLerror();
}

void Circle_HandleKeyboard() {
    float speed = 0.02f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        circleScale.x -= speed;
        if (circleScale.x < 0.1f) circleScale.x = 0.1f;  
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        circleScale.x += speed;
        if (circleScale.x > 3.0f) circleScale.x = 3.0f;  
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        circleScale.y -= speed;
        if (circleScale.y < 0.1f) circleScale.y = 0.1f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        circleScale.y += speed;
        if (circleScale.y > 3.0f) circleScale.y = 3.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        circleScale = glm::vec2(1.0f, 1.0f);
    }
}

void Circle_Draw() {
    Circle_HandleKeyboard();

    glBindBuffer(GL_ARRAY_BUFFER, VBOCircle);
    glUseProgram(ProgramCircle);

    glEnableVertexAttribArray(Attrib_vertex_circle);
    glVertexAttribPointer(Attrib_vertex_circle, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex3DWithColor), (void*)0);

    glEnableVertexAttribArray(Attrib_color_circle);
    glVertexAttribPointer(Attrib_color_circle, 4, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex3DWithColor),
                          (void*)offsetof(Vertex3DWithColor, r));

    glUniform2fv(Uniform_scale, 1, glm::value_ptr(circleScale));

    glDrawArrays(GL_TRIANGLE_FAN, 0, 62); 

    glDisableVertexAttribArray(Attrib_vertex_circle);
    glDisableVertexAttribArray(Attrib_color_circle);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

void Circle_Release() {
    glDeleteBuffers(1, &VBOCircle);
    glDeleteProgram(ProgramCircle);
}