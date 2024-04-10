#ifndef VERTEX_H
#define VERTEX_H
class Vertex3D {
public:
    float x, y, z;
    float r, g, b;
    float nx, ny, nz;

    // Default constructor
    Vertex3D() : x(0), y(0), z(0), r(0), g(0), b(0), nx(0), ny(0), nz(0) {}

    // Constructor with all components
    Vertex3D(float x, float y, float z, float r, float g, float b, float nx, float ny, float nz) 
        : x(x), y(y), z(z), r(r), g(g), b(b), nx(nx), ny(ny), nz(nz) {}
};
#endif