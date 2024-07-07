#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
    Camera(int width, int height, float fov, float near, float far);

    glm::vec3 position;
    glm::vec3 rotation;

    glm::mat4 GetViewMat();
    glm::mat4 GetProjectionMat(){return projection;}

private:
    glm::mat4 projection;
};