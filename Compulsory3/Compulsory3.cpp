#include <glm/glm.hpp>
#include <iostream>

#include "Renderer.h"

// GLFW window size
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char** argv) {
    if (argc == 3) {
        BVM::compileBVMFile(argv[1], argv[2]);
        return 0; // Can be commented to also directly run the program with the newly compiled BVM fully working
    }

    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Compulsory 3", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    Renderer renderer;
    renderer.setAspect(SCR_WIDTH, SCR_HEIGHT);
    Camera gameCamera;
    int test = 0;
    GameState gameState(test);
    gameState.initializeObjects(gameCamera);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    glEnable(GL_DEPTH_TEST);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        gameCamera.processInput(window, deltaTime, lastFrame, gameState.getAllColliders());

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update all objects and render
        gameState.update(deltaTime, gameCamera, window);
        gameState.renderAll(renderer, gameCamera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate resources
    gameState.killAllObjects();
    renderer.cleanup();

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();

    return 0;
}
