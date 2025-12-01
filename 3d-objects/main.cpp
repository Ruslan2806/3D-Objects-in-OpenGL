#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "shader.h"

int main() {
    setlocale(LC_ALL, "ru");

    sf::Window window(sf::VideoMode(1200, 900), "OpenGL Assignments", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
        std::cerr << "GLEW initialization error: " << glewGetErrorString(glewInitResult) << std::endl;
        return -1;
    }

    if (!GLEW_VERSION_3_3) {
        std::cerr << "OpenGL 3.3 not supported!" << std::endl;
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Controls for Tetrahedron (task 1):" << std::endl;
    std::cout << "  Arrow keys - move X/Y" << std::endl;
    std::cout << "  W/S - move Z" << std::endl;

    Init();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    currentAssignment = ASSIGNMENT_1;
                    std::cout << "Switched to Assignment 1: Gradient Tetrahedron" << std::endl;
                }
                else if (event.key.code == sf::Keyboard::Num2) {
                    currentAssignment = ASSIGNMENT_2;
                    std::cout << "Switched to Assignment 2" << std::endl;
                }
                else if (event.key.code == sf::Keyboard::Num3) {
                    currentAssignment = ASSIGNMENT_3;
                    std::cout << "Switched to Assignment 3" << std::endl;
                }
                else if (event.key.code == sf::Keyboard::Num4) {
                    currentAssignment = ASSIGNMENT_4;
                    std::cout << "Switched to Assignment 4" << std::endl;
                }
            }
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Draw();

        window.display();
    }

    Release();

    return 0;
}
