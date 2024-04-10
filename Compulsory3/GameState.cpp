#include "GameState.h"
#include "Renderer.h"

void GameState::update(float deltaTime, Camera& gameCamera, GLFWwindow* window) {
    for (auto& object : objects) {
        auto gameObject = dynamic_cast<GameObject*>(object.get());
        if (gameObject) {
            gameObject->update(deltaTime, window);
        }
        else {
            object->update(deltaTime);
        }
    }

    auto it = objects.begin();
    while (it != objects.end()) 
    {
        auto go = dynamic_cast<GameObject*>(it->get());
        if (go && go->shouldBeRemoved) {
            if (go->GOType == Trophy) trophyCollected(); // Check not necessarily needed in this project context
            it = objects.erase(it); 
        }
        else ++it; // Move to next object if current is not removed
    }
}

void GameState::renderAll(Renderer& renderer, Camera& camera) {
    for (const auto& object : objects) {
        renderer.render(object, camera);
    }
}

// Prepare map objects then add to vector
void GameState::initializeObjects(Camera& camera) {
    BVM terrainModel("terrain.bvm");
    auto terrain = std::make_shared<WorldObject>(
        terrainModel,
        glm::vec3(0.0f, -0.5f, 0.0f), // Position
        glm::vec3(5.0f)               // Scale
    );


    BVM npcModel("npc.bvm");
    auto npc = std::make_shared<NPC>(
        npcModel,
        glm::vec3(-5.f, -0.45f, -5.f), // Position
        glm::vec3(0.2f),               // Scale
        glm::vec3(0.f, 1.f, 0.f),      // Rotation axis (no rotation)
        0.f,                           // Euler rotation angle
        0.1f                           // Speed
    );
    npc->pathPoints = std::vector<glm::vec3> { //NPC path points
    glm::vec3(-5.f, -0.45f, 5.f),
    glm::vec3(-2.f, -0.45f, -2.f),
    glm::vec3(2.f, -0.45f, -2.f),
    glm::vec3(5.f, -0.45f, -5.f)
    };
    BVM objectModel("npc.bvm");
    auto object1 = std::make_shared<WorldObject>(
        objectModel,
        glm::vec3(2.0f, -0.8f, 1.0f), // Position
        glm::vec3(1.0f)               // Scale
    );
    auto object2 = std::make_shared<WorldObject>(
        objectModel,
        glm::vec3(5.0f, -0.8f, -1.0f), // Position
        glm::vec3(1.0f)               // Scale
    );
    auto object3 = std::make_shared<WorldObject>(
        objectModel,
        glm::vec3(-3.0f, -0.8f, -5.0f), // Position
        glm::vec3(1.0f)               // Scale
    );

    addObject(terrain);
    addObject(npc);
    addObject(object1);
    addObject(object2);
    addObject(object3);
}
