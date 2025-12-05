// #################################################### TASK 3 ####################################################

// Кубик с двумя смешанными текстурами. 
// Пропорция должна регулироваться кнопками клавиатуры.

#include "shader.h"
#include <SFML/Graphics.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <filesystem>

GLuint VBO_Cube2;
GLuint ProgramCube2;
GLuint texture1ID;
GLuint texture2ID;
GLuint VAO_Cube2;

static GLuint Attrib_vertex_cube2;
static GLuint Attrib_texcoord_cube2;
static GLuint Uniform_model_cube2;
static GLuint Uniform_view_cube2;
static GLuint Uniform_projection_cube2;
static GLuint Uniform_textureMix_cube2;

float textureMix = 0.5f; // Пропорция смешивания: 0.0 - только текстура 1, 1.0 - только текстура 2

const char* VertexShaderCube2 = R"(
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

const char* FragShaderCube2 = R"(
#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D textureSampler1;
uniform sampler2D textureSampler2;
uniform float textureMix;

void main() {
    vec4 texture1Color = texture(textureSampler1, TexCoord);
    vec4 texture2Color = texture(textureSampler2, TexCoord);
    
    FragColor = mix(texture1Color, texture2Color, textureMix);
}
)";

struct CubeVertex {
    GLfloat x, y, z;
    GLfloat u, v;
};

std::vector<CubeVertex> createCubeVertices2() {
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

std::vector<GLuint> createCubeIndices2() {
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

GLuint loadTexture2(const std::string& filename) {
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    sf::Vector2u size = image.getSize();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        size.x, size.y,
        0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void Cube2_HandleKeyboard() {
    // Управление пропорцией смешивания текстур
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        textureMix -= 0.005f;
        if (textureMix < 0.0f) textureMix = 0.0f;
        std::cout << "Texture mix: " << textureMix << " (0=texture1 only, 1=texture2 only)" << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        textureMix += 0.005f;
        if (textureMix > 1.0f) textureMix = 1.0f;
        std::cout << "Texture mix: " << textureMix << " (0=texture1 only, 1=texture2 only)" << std::endl;
    }
}

void Cube2_InitShader() {
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &VertexShaderCube2, NULL);
    glCompileShader(vShader);
    ShaderLog(vShader);

    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &FragShaderCube2, NULL);
    glCompileShader(fShader);
    ShaderLog(fShader);

    ProgramCube2 = glCreateProgram();
    glAttachShader(ProgramCube2, vShader);
    glAttachShader(ProgramCube2, fShader);
    glLinkProgram(ProgramCube2);

    Attrib_vertex_cube2 = glGetAttribLocation(ProgramCube2, "position");
    if (Attrib_vertex_cube2 == (GLuint)-1) {
        std::cout << "Could not bind attrib position" << std::endl;
        return;
    }

    Attrib_texcoord_cube2 = glGetAttribLocation(ProgramCube2, "texCoord");
    if (Attrib_texcoord_cube2 == (GLuint)-1) {
        std::cout << "Could not bind attrib texCoord" << std::endl;
        return;
    }

    Uniform_model_cube2 = glGetUniformLocation(ProgramCube2, "model");
    if (Uniform_model_cube2 == (GLuint)-1) {
        std::cout << "Could not bind uniform model" << std::endl;
        return;
    }

    Uniform_view_cube2 = glGetUniformLocation(ProgramCube2, "view");
    if (Uniform_view_cube2 == (GLuint)-1) {
        std::cout << "Could not bind uniform view" << std::endl;
        return;
    }

    Uniform_projection_cube2 = glGetUniformLocation(ProgramCube2, "projection");
    if (Uniform_projection_cube2 == (GLuint)-1) {
        std::cout << "Could not bind uniform projection" << std::endl;
        return;
    }

    Uniform_textureMix_cube2 = glGetUniformLocation(ProgramCube2, "textureMix");
    if (Uniform_textureMix_cube2 == (GLuint)-1) {
        std::cout << "Could not bind uniform textureMix" << std::endl;
        return;
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    checkOpenGLerror();
}

void Cube2_InitVBO() {
    auto vertices = createCubeVertices2();
    auto indices = createCubeIndices2();

    glGenVertexArrays(1, &VAO_Cube2);
    glBindVertexArray(VAO_Cube2);

    glGenBuffers(1, &VBO_Cube2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube2);
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

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(CubeVertex), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        sizeof(CubeVertex), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Загрузка двух текстур
    texture1ID = loadTexture2(std::filesystem::path("..") / "3d-objects" / "textures" / "FRUITS.jpg");
    texture2ID = loadTexture2(std::filesystem::path("..") / "3d-objects" / "textures" / "cat-in-orange.jpg");

    checkOpenGLerror();
}

void Cube2_Draw()
{
    Cube2_HandleKeyboard();

    glUseProgram(ProgramCube2);
    glBindVertexArray(VAO_Cube2);

    // Активация и привязка первой текстуры (texture unit 0)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1ID);
    
    // Активация и привязка второй текстуры (texture unit 1)
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2ID);
    
    // Установка uniform-переменных для сэмплеров
    glUniform1i(glGetUniformLocation(ProgramCube2, "textureSampler1"), 0);
    glUniform1i(glGetUniformLocation(ProgramCube2, "textureSampler2"), 1);

    // Матрицы преобразования (можно использовать те же, что и в Task 2)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f), // camera position
        glm::vec3(0.0f, 0.0f, 0.0f), // look at
        glm::vec3(0.0f, 1.0f, 0.0f)  // up vector
    );

    static float rotationAngle = 0.0f;
    rotationAngle += 0.5f;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // Сдвигаем немного вправо относительно первого куба
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotationAngle * 0.7f), glm::vec3(1.0f, 0.0f, 0.0f));

    glUniformMatrix4fv(Uniform_model_cube2, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(Uniform_view_cube2, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(Uniform_projection_cube2, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform1f(Uniform_textureMix_cube2, textureMix);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}

void Cube2_Release() {
    glDeleteBuffers(1, &VBO_Cube2);
    glDeleteProgram(ProgramCube2);
    glDeleteTextures(1, &texture1ID);
    glDeleteTextures(1, &texture2ID);
    glDeleteVertexArrays(1, &VAO_Cube2);
}