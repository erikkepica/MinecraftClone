#include"Applocation.h"

void FrameBufferSizeCallbackG(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width,height);
}
void MouseCallbackG(GLFWwindow *window, double xpos, double ypos)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    app->MouseCallback(xpos, ypos);
}

Application::Application(int width, int height, std::string name)
    :m_Cam(width, height, 45.f, 0.1f, 100.0f, glm::vec3(0,1,0), 10)
{
    m_Width = width;
    m_Height = height;
    m_Name = name;

    m_FirstMouse = true;

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

    glfwSetWindowUserPointer(m_Window, this);

    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(m_Window, FrameBufferSizeCallbackG);
    glfwSetCursorPosCallback(m_Window, MouseCallbackG);
}



void Application::processInput()
{
    if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        m_Cam.RayCast(10);
    }
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_Window, true);
    }
    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
        m_Cam.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
        m_Cam.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
        m_Cam.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
        m_Cam.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_Cam.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        m_Cam.ProcessKeyboard(DOWN, deltaTime);
}

void Application::MouseCallback(double xpos, double ypos)
{
    if (m_FirstMouse) {
        m_LastX = xpos;
        m_LastY = ypos;
        m_FirstMouse = false;
    }

    float xoffset = m_LastX-xpos;
    float yoffset = m_LastY - ypos; // Reversed since y-coordinates range from bottom to top
    m_LastX = xpos;
    m_LastY = ypos;
    m_Cam.ProcessMouseMovement(xoffset, yoffset);
}
