#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include <glm/glm.hpp>

class BoxCollider {
public:
    glm::vec3 minPoint;
    glm::vec3 maxPoint;

    BoxCollider(const glm::vec3& minPoint, const glm::vec3& maxPoint)
        : minPoint(minPoint), maxPoint(maxPoint) {}

};
#endif