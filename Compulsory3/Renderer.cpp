#include "GameObject.h"
#include "Renderer.h"

void Renderer::initializeShaders() {
    std::string vertexShaderSource = ShaderHelper::readShader(VertexSource);
    std::string fragmentShaderSource = ShaderHelper::readShader(FragmentSource);
    shaderProgram = ShaderHelper::createProgram(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
    viewLoc = glGetUniformLocation(shaderProgram, "view");
    projLoc = glGetUniformLocation(shaderProgram, "projection");
    modelLoc = glGetUniformLocation(shaderProgram, "model");
}

void Renderer::render(const std::shared_ptr<WorldObject>& worldObject, const Camera& camera) {
    glUseProgram(shaderProgram);
    updateUniforms(camera);

    // Light properties are simply hardcoded here
    glm::vec3 lightPos = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(camera.position));
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));

    // Calculate and set the model matrix for the current object
    glm::mat4 modelMatrix = worldObject->getModelMatrix();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind the VAO and draw the object
    glBindVertexArray(worldObject->getGLObject(VAOtype));
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(worldObject->model.getIndices().size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}



void Renderer::updateUniforms(const Camera& camera) {
    glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
    glm::mat4 projection = glm::perspective(glm::radians(camera.FoV), SCR_WIDTH / SCR_WIDTH, camera.nearClip, camera.farClip);

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Renderer::cleanup() {
    glDeleteProgram(shaderProgram);
}
