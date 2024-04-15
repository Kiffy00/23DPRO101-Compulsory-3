#include <glm/glm.hpp>
#include <iostream>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h" 


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

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Renderer renderer;
    renderer.setAspect(SCR_WIDTH, SCR_HEIGHT);
    Camera gameCamera;
    GameState gameState(0);
    gameState.initializeObjects(gameCamera);
    gameCamera.baryUtility.initialize(gameState.objects.front()->model.getVertices(), 
                                        gameState.objects.front()->model.getIndices(), 
                                        gameState.objects.front()->getModelMatrix());
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    glEnable(GL_DEPTH_TEST);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        gameCamera.processInput(window, deltaTime, lastFrame, gameState.getAllColliders());

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Light", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
        ImGui::SetWindowPos(ImVec2(SCR_WIDTH - ImGui::GetWindowWidth(), 0)); // Top right
        ImGui::ColorEdit3("Color", glm::value_ptr(renderer.lightColor));
        ImGui::SliderFloat3("Position", glm::value_ptr(renderer.lightPos), -10.0f, 10.0f);
        ImGui::End();

        // Camera height control window (new)
        ImGui::Begin("Camera", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
        ImGui::SetWindowPos(ImVec2(0, 0)); // Top left
        ImGui::SliderFloat("Height", &gameCamera.height, 0.0f, 5.0f);
        ImGui::End();

        // Render ImGui
        ImGui::Render();
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update and render all objects
        gameState.update(deltaTime, gameCamera, window);
        gameState.renderAll(renderer, gameCamera);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
