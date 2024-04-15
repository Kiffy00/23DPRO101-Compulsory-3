#ifndef BARY_UTILITY_H
#define BARY_UTILITY_H

#include <vector>
#include <glm/glm.hpp>
#include "Vertex.h"

class BaryUtility {
public:
    struct TrianglePlane {
        glm::vec3 vertices[3];
        glm::vec3 normal;
        float d; // Ax + By + Cz + D = 0

        TrianglePlane() = default;
        TrianglePlane(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
        float getHeightAt(const glm::vec3& point) const;
    };

    BaryUtility() = default;
    BaryUtility(const std::vector<Vertex3D>& vertices, const std::vector<unsigned int>& indices, const glm::mat4& transform);
    void initialize(const std::vector<Vertex3D>& vertices, const std::vector<unsigned int>& indices, const glm::mat4& transform);

    bool isPointInsideTriangle(const TrianglePlane& trianglePlane, const glm::vec3& point) const;
    float getTerrainHeightAt(const glm::vec3& worldPosition) const;
    std::vector<TrianglePlane> trianglePlanes;
};

#endif
