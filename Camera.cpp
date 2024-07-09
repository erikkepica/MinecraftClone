#include"Camera.h"
#include<iostream>

#include"Game.h"

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
    std::cout << velocity << std::endl;
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

glm::vec3 Camera::RayCast(int distance)
{
    glm::vec3 rayPosition = position;

    Chunk* chunk = Game::GetGame()->GetWorld()->GetChunk(glm::vec2(rayPosition.x,rayPosition.y));

    glm::vec3 invDir = 1.0f / front;

    glm::ivec3 currentBlock = chunk->GetBlock(position);
    glm::vec3 tMax, tDelta;

    glm::ivec3 step;
    for (int i = 0; i < 3; ++i) {
        if (front[i] > 0) {
            tMax[i] = (glm::floor(position[i]) + 1 - position[i]) * invDir[i];
            tDelta[i] = invDir[i];
            step[i] = 1;
        } else {
            tMax[i] = (position[i] - glm::floor(position[i])) * invDir[i];
            tDelta[i] = -invDir[i];
            step[i] = -1;
        }
    }

    float distanceTravelled = 0.0f;

    while (distanceTravelled < distance) {
        if (chunk->GetBlockID(currentBlock)!=0) {
            std::cout << Game::GetGame()->GetBlocks()->GetBlock(chunk->GetBlockID(currentBlock)).GetName() << std::endl;
            return currentBlock;
        }

        if (tMax.x < tMax.y && tMax.x < tMax.z) {
            currentBlock.x += step.x;
            distanceTravelled = tMax.x;
            tMax.x += tDelta.x;
        } else if (tMax.y < tMax.z) {
            currentBlock.y += step.y;
            distanceTravelled = tMax.y;
            tMax.y += tDelta.y;
        } else {
            currentBlock.z += step.z;
            distanceTravelled = tMax.z;
            tMax.z += tDelta.z;
        }
    }

    return glm::ivec3(-999);
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


