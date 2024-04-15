#include "BaryUtility.h"
#include <iostream>

BaryUtility::TrianglePlane::TrianglePlane(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) {
    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;
    normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
    d = -glm::dot(normal, v0);
}

BaryUtility::BaryUtility(const std::vector<Vertex3D>& vertices, const std::vector<unsigned int>& indices, const glm::mat4& transform) {
    for (size_t i = 0; i < indices.size(); i += 3) {
        glm::vec3 v0 = glm::vec3(transform * glm::vec4(vertices[indices[i]].x, vertices[indices[i]].y, vertices[indices[i]].z, 1.0f));
        glm::vec3 v1 = glm::vec3(transform * glm::vec4(vertices[indices[i + 1]].x, vertices[indices[i + 1]].y, vertices[indices[i + 1]].z, 1.0f));
        glm::vec3 v2 = glm::vec3(transform * glm::vec4(vertices[indices[i + 2]].x, vertices[indices[i + 2]].y, vertices[indices[i + 2]].z, 1.0f));

        trianglePlanes.push_back(TrianglePlane(v0, v1, v2));
    }
}
void BaryUtility::initialize(const std::vector<Vertex3D>& vertices, const std::vector<unsigned int>& indices, const glm::mat4& transform) {
    trianglePlanes.clear();
    for (size_t i = 0; i < indices.size(); i += 3) {
        glm::vec3 v0 = glm::vec3(transform * glm::vec4(vertices[indices[i]].x, vertices[indices[i]].y, vertices[indices[i]].z, 1.0f));
        glm::vec3 v1 = glm::vec3(transform * glm::vec4(vertices[indices[i + 1]].x, vertices[indices[i + 1]].y, vertices[indices[i + 1]].z, 1.0f));
        glm::vec3 v2 = glm::vec3(transform * glm::vec4(vertices[indices[i + 2]].x, vertices[indices[i + 2]].y, vertices[indices[i + 2]].z, 1.0f));

        trianglePlanes.push_back(TrianglePlane(v0, v1, v2));
    }
}

float BaryUtility::TrianglePlane::getHeightAt(const glm::vec3& point) const {
    return - (normal.x * point.x + normal.z * point.z + d) / normal.y;
}

bool BaryUtility::isPointInsideTriangle(const TrianglePlane& trianglePlane, const glm::vec3& point) const {
    // Compute vectors from triangle vertices
    glm::vec3 v0 = trianglePlane.vertices[1] - trianglePlane.vertices[0];
    glm::vec3 v1 = trianglePlane.vertices[2] - trianglePlane.vertices[0];
    glm::vec3 v2 = point - trianglePlane.vertices[0];

    // Compute dot products
    float dot00 = glm::dot(v0, v0);
    float dot01 = glm::dot(v0, v1);
    float dot02 = glm::dot(v0, v2);
    float dot11 = glm::dot(v1, v1);
    float dot12 = glm::dot(v1, v2);

    // Compute barycentric coordinates
    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if point is in triangle
    return (u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.0f);
}


float BaryUtility::getTerrainHeightAt(const glm::vec3& worldPosition) const {
    float closestHeight = std::numeric_limits<float>::lowest();

    for (const auto& plane : trianglePlanes) {
        float planeHeightAtPoint = plane.getHeightAt(worldPosition);
        glm::vec3 projectedPoint(worldPosition.x, planeHeightAtPoint, worldPosition.z);

        if (isPointInsideTriangle(plane, projectedPoint)) {
            // Update if right triangle
            if (closestHeight < planeHeightAtPoint) {
                closestHeight = planeHeightAtPoint;
            }
        }
    }

    // No triangle was found above or below the point -- default to 0.0f height
    return (closestHeight == std::numeric_limits<float>::lowest()) ? 0.0f : closestHeight;
}

