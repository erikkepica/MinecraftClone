#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera
{
public:
    Camera(int width, int height, float fov, float near, float far, glm::vec3 worldUp_, float movementSpeed);

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    glm::mat4 GetViewMat();
    glm::mat4 GetProjectionMat(){return projection;}

private:
    void UpdateCameraVectors();
    float m_MovementSpeed;
    glm::mat4 projection;
};