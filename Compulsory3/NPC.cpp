#include "NPC.h"

void NPC::update(float deltaTime) {
    followCurve(deltaTime);
    position = calculateLagrangePosition(this->pathPoints, t);

    // Map to terrain
    position.y = baryUtility.getTerrainHeightAt(position) + (1.0 * scale.x); 
}

void NPC::followCurve(float deltaTime) {
    if (movingForward) {
        t += deltaTime * speed;
        if (t >= 1.0f) {
            t = 1.0f;
            movingForward = false;
        }
    }
    else {
        t -= deltaTime * speed;
        if (t <= 0.0f) {
            t = 0.0f;
            movingForward = true;
        }
    }
}

glm::vec3 NPC::calculateLagrangePosition(const std::vector<glm::vec3>& points, float t) {
    glm::vec3 result(0.0f);

    int n = points.size();
    for (int i = 0; i < n; ++i) {
        float L = 1.0f;
        for (int j = 0; j < n; ++j) {
            if (j != i) {
                L *= (t - (float)j / (n - 1)) / ((float)i / (n - 1) - (float)j / (n - 1));
            }
        }
        result += L * points[i];
    }

    return result;
}