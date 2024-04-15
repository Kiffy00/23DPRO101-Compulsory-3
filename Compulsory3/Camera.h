#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "GameObject.h"

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float height{1.0f};
    float yaw;
    float pitch;
    float movementSpeed{ 2.f };
    float lookSensitivity{ 100.f };
    float nearClip{ 0.1f };
    float farClip{ 100.f };
    float FoV{ 75 };
    bool disableInput{ false };

    BaryUtility baryUtility;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), 
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
        float yaw = -90.0f, float pitch = 0.0f)
        : position(position), front(front), up(up), yaw(yaw), pitch(pitch) {};

    void setInputEnabled(bool enabled) {
        disableInput = !enabled;
    }

    void updateCameraDirection() {
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
        up = glm::normalize(glm::cross(right, front));
    }

    bool checkCameraCollision(glm::vec3 pos, const BoxCollider& collider) {
        bool collisionX = pos.x >= collider.minPoint.x && pos.x <= collider.maxPoint.x;
        bool collisionY = pos.y >= collider.minPoint.y && pos.y <= collider.maxPoint.y;
        bool collisionZ = pos.z >= collider.minPoint.z && pos.z <= collider.maxPoint.z;
        return collisionX && collisionY && collisionZ;
    }

    void processInput(GLFWwindow* window, float& deltaTime, float& lastFrame, const std::vector<BoxCollider>& colliders) {
        if (disableInput) return;

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        float cameraSpeed = movementSpeed * deltaTime;
        glm::vec3 moveDirection = glm::vec3(0.0f);

        // Limit to horizontal movement
        glm::vec3 cameraFrontHorizontal = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
        glm::vec3 cameraRightHorizontal = glm::normalize(glm::cross(cameraFrontHorizontal, up));

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            moveDirection += cameraFrontHorizontal;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            moveDirection -= cameraFrontHorizontal;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            moveDirection -= cameraRightHorizontal;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            moveDirection += cameraRightHorizontal;

        // Normalize movement speed
        if (glm::length(moveDirection) > 0) {
            moveDirection = glm::normalize(moveDirection);
        }

        glm::vec3 newPosition = position + moveDirection * cameraSpeed;
        // Check for potential collisions with the new position
        for (const auto& collider : colliders) {
            if (checkCameraCollision(newPosition, collider)) {
                newPosition = position - moveDirection * (cameraSpeed); // Move back slightly in the opposite direction of movement
                break;
            }
        }
        newPosition.y = baryUtility.getTerrainHeightAt(newPosition) + height;
        position = newPosition;

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) pitch += lookSensitivity * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) pitch -= lookSensitivity * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) yaw -= lookSensitivity * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) yaw += lookSensitivity * deltaTime;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        glm::vec3 direction = glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
        front = glm::normalize(direction);
    }

    // Members and methods for camera snap //
    glm::vec3 savedPosition;
    glm::vec3 savedFront;
    glm::vec3 savedUp;

    void saveState() {
        savedPosition = position;
        savedFront = front;
        savedUp = up;
    }
    void restoreState() {
        position = savedPosition;
        front = savedFront;
        up = savedUp;
    }
    // Members and methods for camera snap //
};
#endif