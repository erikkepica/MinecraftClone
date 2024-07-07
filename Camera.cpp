#include"Camera.h"

Camera::Camera(int width, int height, float fov, float near, float far)
{
    position = glm::vec3(0,0,-3);

    rotation = glm::vec3(0,0,0);

    projection = glm::perspective(glm::radians(fov), (float)width/(float)height, near, far);
}

glm::mat4 Camera::GetViewMat()
{
    glm::mat4 view(1.0f);


    view = glm::rotate(view, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
    view = glm::rotate(view, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
    view = glm::rotate(view, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));

    view = glm::translate(view, position);

    return view;
}