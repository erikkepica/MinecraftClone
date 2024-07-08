#include"Camera.h"
#include<iostream>

Camera::Camera(int width, int height, float fov, float near, float far, glm::vec3 worldUp_, float movementSpeed):
    front(glm::vec3(0.0f, 0.0f, -1.0f)), 
    worldUp(worldUp_), 
    m_MovementSpeed(movementSpeed)
{
    position = glm::vec3(0,0,0);

    yaw = 0.f;
    pitch = 0.f;

    projection = glm::perspective(glm::radians(fov), (float)width/(float)height, near, far);

    UpdateCameraVectors();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;


    yaw += -xoffset;
    pitch += yoffset;


    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }
    if (yaw > 360.0f)
        yaw -= 360.0f;
    if (yaw < -360.0f)
        yaw += 360.0f;


    UpdateCameraVectors();
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = m_MovementSpeed * deltaTime;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
    if (direction == UP)
        position += up * velocity;
    if (direction == DOWN)
        position -= up * velocity;
}

glm::mat4 Camera::GetViewMat()
{
    return glm::lookAt(position, position + front, up);
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 front_;
    front_.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front_.y = sin(glm::radians(pitch));
    front_.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front_);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
