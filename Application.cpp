#include"Applocation.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width,height);
}

Application::Application(int width, int height, std::string name)
    :m_Cam(width, height, 45.f, 0.1f, 100.0f)
{
    m_Width = width;
    m_Height = height;
    m_Name = name;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    m_Window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (m_Window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    
    glfwMakeContextCurrent(m_Window);
    glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
}



void Application::processInput()
{
    if(glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        StopApplication();

    if(glfwGetKey(m_Window, GLFW_KEY_V) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if(glfwGetKey(m_Window, GLFW_KEY_B) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);

    int rotDirX = glfwGetKey(m_Window, GLFW_KEY_RIGHT)-glfwGetKey(m_Window, GLFW_KEY_LEFT);
    int rotDirY = glfwGetKey(m_Window, GLFW_KEY_UP)-glfwGetKey(m_Window, GLFW_KEY_DOWN);

    int movDirX = (glfwGetKey(m_Window, GLFW_KEY_D)-glfwGetKey(m_Window, GLFW_KEY_A));
    int movDirZ= (glfwGetKey(m_Window, GLFW_KEY_W)-glfwGetKey(m_Window, GLFW_KEY_S));
    int movDirY= (glfwGetKey(m_Window, GLFW_KEY_SPACE)-glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT));

    m_Cam.rotation+=glm::vec3(-rotDirY,rotDirX,0);
    m_Cam.position+=glm::vec3((float)-movDirX/10,(float)-movDirY/10,(float)movDirZ/10);
}