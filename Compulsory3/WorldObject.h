#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "BinaryVertexModel.h"
#include "BoxCollider.h"
class Renderer;

enum GLObjectEnum {
    VAOtype,
    VBOtype,
    EBOtype
};

class WorldObject {
public:
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotationAxis; // Axis for rotation
    float rotationAngle; // Rotation angle in euler
    BVM model;
    std::vector<BoxCollider> colliders;

    WorldObject(BVM& model, const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.f), const glm::vec3& rotAxis = glm::vec3(0.f, 1.f, 0.f), float rotAngle = 0.0f);

    virtual ~WorldObject() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    virtual void update(float deltaTime) {
        // Base implementation does nothing
        // Overrided in derived classes
    }

    GLuint getGLObject(GLObjectEnum object) const {
        switch (object) {
        case VAOtype:
            return VAO;
        case VBOtype:
            return VBO;
        case EBOtype:
            return EBO;
        default:
            std::cerr << "Unknown GLObjectEnum value." << std::endl;
            return 0;
        }
    }

    glm::mat4 getModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);
        model = glm::scale(model, scale);
        return model;
    }

    void addColliderBounds(const BoxCollider& collider) {
        colliders.push_back(collider);
    }

    void updateColliderPositions(const glm::vec3& delta) {
        for (auto& collider : colliders) {
            collider.minPoint += delta;
            collider.maxPoint += delta;
        }
    }

private:
    GLuint VAO{ 0 }, VBO{ 0 }, EBO{ 0 };
};
#endif
