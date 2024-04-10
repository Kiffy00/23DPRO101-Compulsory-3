#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "Vertex.h"

class BVM { // Binary Vertex Model (custom format)
    const std::string magicHeader = "BVM";
    std::vector<Vertex3D> vertices;
    std::vector<unsigned int> indices;

public:
    BVM(const std::string& filePath) 
    { loadFromFile(filePath); }

    const std::vector<Vertex3D>& getVertices() 
    { return vertices; }
    const std::vector<unsigned int>& getIndices()
    { return indices; }

    void loadFromFile(const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary);
    
        if (!file) {
            std::cerr << "Failed to open file: " << filePath << std::endl;
            return;
        }
    
        char header[4] = { 0 };
        file.read(header, 3);
        if (std::string(header, 3) != magicHeader) {
            std::cerr << "Invalid file format on " << filePath <<  "." << std::endl;
            return;
        }
    
        unsigned int numVertices;
        file.read(reinterpret_cast<char*>(&numVertices), sizeof(numVertices));
    
        // Reserve space for the vertices to avoid reallocations
        vertices.reserve(numVertices);
    
        for (unsigned int i = 0; i < numVertices; ++i) {
            Vertex3D vertex;
            file.read(reinterpret_cast<char*>(&vertex), sizeof(Vertex3D));
            vertices.push_back(vertex);
        }
    
        unsigned int numIndices;
        file.read(reinterpret_cast<char*>(&numIndices), sizeof(numIndices));
    
        indices.resize(numIndices);
        file.read(reinterpret_cast<char*>(indices.data()), numIndices * sizeof(unsigned int));
    
        file.close();
    }
    
    static void compileBVMFile(const std::string& inputTextFile, const std::string& outputBVMFile) { // Triangulated indices
        std::ifstream textFile(inputTextFile);
        std::ofstream bvmFile(outputBVMFile, std::ios::binary);
        std::string line;
        float x, y, z, r, g, b, nx, ny, nz;
        unsigned int idx1, idx2, idx3;

        // Raw float data of vertices and indices
        std::vector<float> vertexData; 
        std::vector<unsigned int> indices; 

        if (!textFile) {
            std::cerr << "Could not open text file for reading." << std::endl; return;
        } else
            std::cout << "Opened text file for reading." << std::endl;

        if (!bvmFile) {
            std::cerr << "Could not open BVM file for writing." << std::endl; return;
        } else
            std::cout << "Opened BVM file for writing." << std::endl;

        // First pass to count vertices and indices
        while (std::getline(textFile, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;
            char comma; // Ignore commas
            if (prefix == "vcn" && (iss >> x >> comma >> y >> comma >> z >> comma >> r >> comma >> g >> comma >> b >> comma >> nx >> comma >> ny >> comma >> nz)) {
                vertexData.insert(vertexData.end(), { x, y, z, r, g, b, nx, ny, nz });
            }
            else if (prefix == "idx" && (iss >> idx1 >> comma >> idx2 >> comma >> idx3)) {
                indices.insert(indices.end(), { idx1, idx2, idx3 });
            }
        }

        // Write magic header
        bvmFile.write("BVM", 3);

        // Write number of vertices
        unsigned int numVertices = vertexData.size() / 9;
        bvmFile.write(reinterpret_cast<char*>(&numVertices), sizeof(numVertices));

        // Write vertex data
        bvmFile.write(reinterpret_cast<char*>(vertexData.data()), vertexData.size() * sizeof(float));

        // Write number of indices
        unsigned int numIndices = indices.size();
        bvmFile.write(reinterpret_cast<char*>(&numIndices), sizeof(numIndices));

        // Write index data
        bvmFile.write(reinterpret_cast<char*>(indices.data()), indices.size() * sizeof(unsigned int));

        std::cout << "Compiled " << numVertices << " vertices and " << numIndices / 3 << " triangle indices to BVM file." << std::endl;
    }
};