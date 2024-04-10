#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <vector>

enum shaderEnum {
    VertexSource,
    FragmentSource
};

class ShaderHelper
{
public:
    static GLuint compileShader(const GLchar* source, GLenum type) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        return shader;
    }

    static GLuint createProgram(const GLchar* vertexSource, const GLchar* fragmentSource) {
        GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
        GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

        if (!vertexShader || !fragmentShader) {
            return 0; // Failed to compile
        }

        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        // Clean up
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }

    template <typename T, typename I>
    static void setupObjects(GLuint& VAO, GLuint& VBO, GLuint& EBO, const std::vector<T>& vertices, const std::vector<I>& indices) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // Bind and set the vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);

        // Position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(T), (void*)0);

        // Color attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(T), (void*)(3 * sizeof(float)));

        // Normal attribute
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(T), (void*)(6 * sizeof(float)));

        // Bind and set the element buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(I), indices.data(), GL_STATIC_DRAW);

        // Unbind the VAO
        glBindVertexArray(0);

        // Unbind the VBO and EBO to avoid accidental modifications
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }


    // reads from current folder
    static std::string readShader(bool fragment) {
        std::ifstream shaderFile;
        if (fragment)
            shaderFile.open("fragmentShaderSource.frag");
        else
            shaderFile.open("vertexShaderSource.vert");

        std::string str((std::istreambuf_iterator<char>(shaderFile)),
            std::istreambuf_iterator<char>());
        return str;
    }
};

