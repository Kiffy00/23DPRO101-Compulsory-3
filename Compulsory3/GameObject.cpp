#include "GameObject.h"

bool GameObject::checkInteraction() {
    float distance = glm::length(camera.position - position);
    return distance <= interactRange;
}
void GameObject::update(float deltaTime, GLFWwindow* window) {
    if (isInteractable && !isInteracting && checkInteraction() && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        if (GOType == Trophy) {
            shouldBeRemoved = true;
        }
        else if (canInteract && !isInteracting) { // Snap camera
            camera.saveState(); // Save the camera's current state before snapping.

            camera.position = glm::vec3(0.0f, 0.2f, -10.0f);
            camera.front = glm::vec3(0.0f, 0.0f, 1.0f);
            camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
            camera.disableInput = true;

            isInteracting = true;
            interactionTimer = 0.0f;

        }
    }

    if (isInteracting) {
        interactionTimer += deltaTime;
        if (interactionTimer <= 2.0f) {
            float moveY = deltaTime;
            position.y += moveY;
            updateColliderPositions(glm::vec3(0.0f, moveY, 0.0f));
        }
        else { // Interaction has finished
            isInteracting = false;
            interactionTimer = 0.0f;

            camera.restoreState(); // Restore the camera's orientation and position.
            camera.disableInput = false;
            isInteracting = false;

            isInteracting = false;
            interactionTimer = 0.0f;
        }
    }
}