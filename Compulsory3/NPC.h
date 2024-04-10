#include "GameObject.h"

class NPC : public WorldObject {
public:
    bool movingForward{ true };
    float speed{ 1.0f };
    std::vector<glm::vec3> pathPoints;
    float t = 0.0f; // Parameter for curve position

    NPC(BVM& model, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotAxis, float rotAngle , float speed = 1.0f)
        : WorldObject(model, pos, scale, rotAxis, rotAngle), speed(speed) {}

    void update(float deltaTime);
    void followCurve(float deltaTime);
    glm::vec3 calculateLagrangePosition(const std::vector<glm::vec3>& points, float t);
};