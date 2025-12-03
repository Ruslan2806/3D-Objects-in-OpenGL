// #################################################### TASK 2 ####################################################

// Кубик с наложенной на него текстурой. 
// Текстура должна быть соединена с цветами, влияние цвета регулируется кнопками клавиатуры.

#include "shader.h"
#include <SFML/Graphics.hpp>
#include <glm/gtc/matrix_transform.hpp>

GLuint VBO_Cube1;
GLuint ProgramCube1;
GLuint textureID;
GLuint VAO_Cube1;

static GLuint Attrib_vertex_cube1;
static GLuint Attrib_texcoord_cube1;
static GLuint Uniform_model_cube1;
static GLuint Uniform_view_cube1;
static GLuint Uniform_projection_cube1;
static GLuint Uniform_colorIntensity_cube1;
static GLuint Uniform_baseColor_cube1;

float colorIntensity = 1.0f; // Интенсивность цвета: 0.0 - только текстура, 1.0 - цвет * текстура
glm::vec3 baseColor = glm::vec3(1.0f, 1.0f, 1.0f); // color to prod

const char* VertexShaderCube1 = R"(
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    TexCoord = texCoord;
}
)";

const char* FragShaderCube1 = R"(
#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D textureSampler;
uniform float colorIntensity;
uniform vec3 baseColor;

void main() {
    vec4 textureColor = texture(textureSampler, TexCoord);
    
    vec4 coloredTexture = vec4(textureColor.rgb * baseColor, textureColor.a);
    
    FragColor = mix(textureColor, coloredTexture, colorIntensity);
}
)";

struct CubeVertex {
    GLfloat x, y, z;
    GLfloat u, v;
};

std::vector<CubeVertex> createCubeVertices() {
    std::vector<CubeVertex> vertices;

    // coord(1,2,3) textcoord(4,5)
    // face
    vertices.push_back({ -0.5f, -0.5f,  0.5f, 0.0f, 0.0f });
    vertices.push_back({ 0.5f, -0.5f,  0.5f, 1.0f, 0.0f });
    vertices.push_back({ 0.5f,  0.5f,  0.5f, 1.0f, 1.0f });
    vertices.push_back({ -0.5f,  0.5f,  0.5f, 0.0f, 1.0f });

    // back
    vertices.push_back({ -0.5f, -0.5f, -0.5f, 1.0f, 0.0f });
    vertices.push_back({ -0.5f,  0.5f, -0.5f, 1.0f, 1.0f });
    vertices.push_back({ 0.5f,  0.5f, -0.5f, 0.0f, 1.0f });
    vertices.push_back({ 0.5f, -0.5f, -0.5f, 0.0f, 0.0f });

    // up
    vertices.push_back({ -0.5f,  0.5f, -0.5f, 0.0f, 1.0f });
    vertices.push_back({ -0.5f,  0.5f,  0.5f, 0.0f, 0.0f });
    vertices.push_back({ 0.5f,  0.5f,  0.5f, 1.0f, 0.0f });
    vertices.push_back({ 0.5f,  0.5f, -0.5f, 1.0f, 1.0f });

    // down
    vertices.push_back({ -0.5f, -0.5f, -0.5f, 1.0f, 1.0f });
    vertices.push_back({ 0.5f, -0.5f, -0.5f, 0.0f, 1.0f });
    vertices.push_back({ 0.5f, -0.5f,  0.5f, 0.0f, 0.0f });
    vertices.push_back({ -0.5f, -0.5f,  0.5f, 1.0f, 0.0f });

    // right
    vertices.push_back({ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f });
    vertices.push_back({ 0.5f,  0.5f, -0.5f, 1.0f, 1.0f });
    vertices.push_back({ 0.5f,  0.5f,  0.5f, 0.0f, 1.0f });
    vertices.push_back({ 0.5f, -0.5f,  0.5f, 0.0f, 0.0f });

    // left
    vertices.push_back({ -0.5f, -0.5f, -0.5f, 0.0f, 0.0f });
    vertices.push_back({ -0.5f, -0.5f,  0.5f, 1.0f, 0.0f });
    vertices.push_back({ -0.5f,  0.5f,  0.5f, 1.0f, 1.0f });
    vertices.push_back({ -0.5f,  0.5f, -0.5f, 0.0f, 1.0f });

    return vertices;
}

std::vector<GLuint> createCubeIndices() {
    std::vector<GLuint> indices;

    for (int i = 0; i < 6; i++) {
        GLuint ind = i * 4;
        indices.push_back(ind);
        indices.push_back(ind + 1);
        indices.push_back(ind + 2);
        indices.push_back(ind);
        indices.push_back(ind + 2);
        indices.push_back(ind + 3);
    }

    return indices;
}

GLuint loadTexture(const std::string& filename) {
    sf::Image image;
    if (!image.loadFromFile(filename)) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        image.create(256, 256, sf::Color::Cyan);
        for (int y = 0; y < 256; y++) {
            for (int x = 0; x < 256; x++) {
                if ((x / 32 + y / 32) % 2 == 0) {
                    image.setPixel(x, y, sf::Color::Magenta);
                }
            }
        }
    }

    image.flipVertically(); // OpenGL ожидает текстуры снизу-вверх

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // if uv coord >1 or <0
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT

    sf::Vector2u size = image.getSize();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        size.x, size.y,
        0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void Cube1_HandleKeyboard() {
    // intensity
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        colorIntensity -= 0.01f;
        if (colorIntensity < 0.0f) colorIntensity = 0.0f;
        std::cout << "Color intensity: " << colorIntensity << " (0=only texture, 1=color*texture)" << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        colorIntensity += 0.01f;
        if (colorIntensity > 1.0f) colorIntensity = 1.0f;
        std::cout << "Color intensity: " << colorIntensity << " (0=only texture, 1=color*texture)" << std::endl;
    }

    // colors
    // red 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        baseColor.r += 0.02f;
        if (baseColor.r > 2.0f) baseColor.r = 2.0f;
        std::cout << "Base color: R=" << baseColor.r << " G=" << baseColor.g << " B=" << baseColor.b << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        baseColor.r -= 0.02f;
        if (baseColor.r < 0.0f) baseColor.r = 0.0f;
        std::cout << "Base color: R=" << baseColor.r << " G=" << baseColor.g << " B=" << baseColor.b << std::endl;
    }

    // green
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        baseColor.g += 0.02f;
        if (baseColor.g > 2.0f) baseColor.g = 2.0f;
        std::cout << "Base color: R=" << baseColor.r << " G=" << baseColor.g << " B=" << baseColor.b << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        baseColor.g -= 0.02f;
        if (baseColor.g < 0.0f) baseColor.g = 0.0f;
        std::cout << "Base color: R=" << baseColor.r << " G=" << baseColor.g << " B=" << baseColor.b << std::endl;
    }

    // blue
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        baseColor.b += 0.02f;
        if (baseColor.b > 2.0f) baseColor.b = 2.0f;
        std::cout << "Base color: R=" << baseColor.r << " G=" << baseColor.g << " B=" << baseColor.b << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        baseColor.b -= 0.02f;
        if (baseColor.b < 0.0f) baseColor.b = 0.0f;
        std::cout << "Base color: R=" << baseColor.r << " G=" << baseColor.g << " B=" << baseColor.b << std::endl;
    }

    // clear
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        colorIntensity = 1.0f;
        baseColor = glm::vec3(1.0f, 1.0f, 1.0f);
        std::cout << "Reset to defaults: white color * texture" << std::endl;
    }

    // only texture
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
        colorIntensity = 0.0f;
        std::cout << "Texture only (no color)" << std::endl;
    }
}

void Cube1_InitShader() {
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &VertexShaderCube1, NULL);
    glCompileShader(vShader);
    ShaderLog(vShader);

    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &FragShaderCube1, NULL);
    glCompileShader(fShader);
    ShaderLog(fShader);

    ProgramCube1 = glCreateProgram();
    glAttachShader(ProgramCube1, vShader);
    glAttachShader(ProgramCube1, fShader);
    glLinkProgram(ProgramCube1);

    Attrib_vertex_cube1 = glGetAttribLocation(ProgramCube1, "position");
    if (Attrib_vertex_cube1 == (GLuint)-1) {
        std::cout << "Could not bind attrib position" << std::endl;
        return;
    }

    Attrib_texcoord_cube1 = glGetAttribLocation(ProgramCube1, "texCoord");
    if (Attrib_texcoord_cube1 == (GLuint)-1) {
        std::cout << "Could not bind attrib texCoord" << std::endl;
        return;
    }

    Uniform_model_cube1 = glGetUniformLocation(ProgramCube1, "model");
    if (Uniform_model_cube1 == (GLuint)-1) {
        std::cout << "Could not bind uniform model" << std::endl;
        return;
    }

    Uniform_view_cube1 = glGetUniformLocation(ProgramCube1, "view");
    if (Uniform_view_cube1 == (GLuint)-1) {
        std::cout << "Could not bind uniform view" << std::endl;
        return;
    }

    Uniform_projection_cube1 = glGetUniformLocation(ProgramCube1, "projection");
    if (Uniform_projection_cube1 == (GLuint)-1) {
        std::cout << "Could not bind uniform projection" << std::endl;
        return;
    }

    Uniform_colorIntensity_cube1 = glGetUniformLocation(ProgramCube1, "colorIntensity");
    if (Uniform_colorIntensity_cube1 == (GLuint)-1) {
        std::cout << "Could not bind uniform colorIntensity" << std::endl;
        return;
    }

    Uniform_baseColor_cube1 = glGetUniformLocation(ProgramCube1, "baseColor");
    if (Uniform_baseColor_cube1 == (GLuint)-1) {
        std::cout << "Could not bind uniform baseColor" << std::endl;
        return;
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    checkOpenGLerror();
}

void Cube1_InitVBO() {
    auto vertices = createCubeVertices();
    auto indices = createCubeIndices();

    glGenVertexArrays(1, &VAO_Cube1);
    glBindVertexArray(VAO_Cube1);

    glGenBuffers(1, &VBO_Cube1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube1);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(CubeVertex),
        vertices.data(),
        GL_STATIC_DRAW);

    GLuint IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(GLuint),
        indices.data(),
        GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(CubeVertex), (void*)0);
    glEnableVertexAttribArray(0);

    // text coord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        sizeof(CubeVertex), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    textureID = loadTexture("textures\\FRUITS.jpg");
    //textureID = loadTexture("textures\\test.jpg");

    checkOpenGLerror();
}

void Cube1_Draw()
{
    Cube1_HandleKeyboard();

    glUseProgram(ProgramCube1);
    glBindVertexArray(VAO_Cube1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f), //camera pos
        glm::vec3(0.0f, 0.0f, 0.0f), // look at
        glm::vec3(0.0f, 1.0f, 0.0f) // up
    );

    static float rotationAngle = 0.0f;
    rotationAngle += 0.5f;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotationAngle * 0.7f), glm::vec3(1.0f, 0.0f, 0.0f));

    glUniformMatrix4fv(Uniform_model_cube1, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(Uniform_view_cube1, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(Uniform_projection_cube1, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform1f(Uniform_colorIntensity_cube1, colorIntensity);
    glUniform3fv(Uniform_baseColor_cube1, 1, glm::value_ptr(baseColor));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}

void Cube1_Release() {
    glDeleteBuffers(1, &VBO_Cube1);
    glDeleteProgram(ProgramCube1);
    glDeleteTextures(1, &textureID);
    glDeleteVertexArrays(1, &VAO_Cube1);
}