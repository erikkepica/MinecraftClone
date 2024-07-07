#pragma once
#include <GLFW/glfw3.h>

#include<iostream>
#include<string>
#include<memory>

#include"Camera.h"

class Application
{
public:
    Application(int width, int height, std::string name);

    //GETERS
    GLFWwindow* GetWindow(){ return m_Window; }
    Camera GetCam(){return m_Cam;}

    void processInput();

    void StopApplication() {glfwSetWindowShouldClose(m_Window, true);}

private:
    int m_Width;
    int m_Height;
    std::string m_Name;

    GLFWwindow* m_Window;

    Camera m_Cam;
};