#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "WorldObject.h"
#include "Camera.h"

class Camera;
enum GOTypeEnum {
    Trophy,
    Door
};

class GameObject : public WorldObject {
public:
    GOTypeEnum GOType{ Trophy };
    bool isInteractable;
    float interactRange;
    float interactionTimer{ 0.0f };
    bool isInteracting{ false };
    bool canInteract{ false };
    bool shouldBeRemoved{ false };

    GLFWwindow* window;
    Camera& camera;

    GameObject(BVM& model, Camera& camera, GOTypeEnum GOType = Trophy, const glm::vec3& pos = glm::vec3(0.f), const glm::vec3& scale = glm::vec3(1.f),
        const glm::vec3& rotAxis = glm::vec3(0.0f, 1.0f, 0.0f), float rotAngle = 0.f,
        bool interactable = true, bool canInteract = false, float range = 1.f)
        : WorldObject(model, pos, scale, rotAxis, rotAngle), camera(camera),
        isInteractable(interactable), interactRange(range), GOType(GOType), canInteract(canInteract) {}

    bool checkInteraction();

    void update(float deltaTime, GLFWwindow* window);
    void update(float deltaTime) override { update(deltaTime, window); }
};
#endif