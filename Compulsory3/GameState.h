#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <memory>
#include "GameObject.h"
#include "NPC.h"

class Renderer;
class Camera;

class GameState {
public:
    std::vector<std::shared_ptr<WorldObject>> objects;
    unsigned int numTrophies{ 0 };
    unsigned int trophiesCollected{ 0};

    GameState(int numTrophies) : numTrophies(numTrophies) {}

    void addObject(const std::shared_ptr<WorldObject>& object) {
        objects.push_back(object);
    }

    std::shared_ptr<GameObject> createTrophy(const glm::vec3& position, const glm::vec3& size, Camera& camera) {
        BVM model("trophy.bvm");
        auto trophy = std::make_shared<GameObject>(model, camera, Trophy, position, size,  glm::vec3(0.f, 1.f, 0.f), ((float)rand() / (RAND_MAX)) * 360); // Randomized rotation each run
        return trophy;
    }

    std::vector<BoxCollider> getAllColliders() const {
        std::vector<BoxCollider> colliders;
        for (const auto& object : objects) {
            const auto& objectColliders = object->colliders;
            colliders.insert(colliders.end(), objectColliders.begin(), objectColliders.end());
        }
        return colliders;
    }

    void update(float deltaTime, Camera& gameCamera, GLFWwindow* window);

    void trophyCollected() {
        trophiesCollected++;
        if (trophiesCollected == numTrophies) { // Find and enable door interaction
            for (auto& object : objects) {
                auto gameObject = dynamic_cast<GameObject*>(object.get());
                if (gameObject && gameObject->GOType == Door) {
                    gameObject->canInteract = true;
                    break;
                }
            }
        }
    }
    // Render all world objects in the game state
    void renderAll(Renderer& renderer, Camera& camera);
    // Arbitrary initialization of world and game objects
    void initializeObjects(Camera& camera);

    void killAllObjects() {
        for (const auto& object : objects) 
            object->~WorldObject();
        objects.clear();
    }
};
#endif