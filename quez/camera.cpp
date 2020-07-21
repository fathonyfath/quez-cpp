#include "camera.h"

#include "game_engine.h"

using namespace std;

Camera::Camera(float screenWidth, float screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    this->lookAt = glm::vec3(0.0f, 0.0f, 1.0f);
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);

    this->halfScreen = glm::vec3(screenWidth / 2.0f, -(screenHeight / 2.0f), 0);

    this->projection = glm::ortho(0.0f, screenWidth, 0.0f, screenHeight);
    this->viewport = glm::vec4(0, 0, screenWidth, screenHeight);
    this->view = glm::lookAt(this->position + this->halfScreen, this->position + this->halfScreen + this->lookAt, this->up);
}

void Camera::updateShaderViewProjection(Shader& shader) {
    shader.setMatrix4("view", this->view, true);
    shader.setMatrix4("projection", this->projection, true);
}

glm::vec3 Camera::screenToWorldSpace(glm::vec2 screenSpace) {
    return glm::unProject(glm::vec3(screenSpace.x, screenSpace.y, 1.0f), this->view, this->projection, this->viewport);
}