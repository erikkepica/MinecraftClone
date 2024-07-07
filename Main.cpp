#include "glad/glad.h"

#include"Applocation.h"
#include"Shader.h"
#include"Graphics.h"
#include"Chunk.h"
#include"Atlas.h"

#include <iostream>



// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    
    Application app(SCR_WIDTH,SCR_HEIGHT, "Minecraft Clone");

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    glEnable(GL_DEPTH_TEST);


    Blocks::AddBlock(Block("GRASS",std::array<glm::vec2, 4>{
        glm::vec2(0,0),
        glm::vec2(0.5f,0),
        glm::vec2(0.5,1.0f),
        glm::vec2(0,1.0f)},
    false));
    Blocks::AddBlock(Block("STONE", std::array<glm::vec2, 4>{
        glm::vec2(0.5f,0),
        glm::vec2(1.0f,0),
        glm::vec2(1.0f,1.0f),
        glm::vec2(0.5f,1.0f)},
    false));



    Shader shader("shaders/vert.glsl", "shaders/frag.glsl");
    Texture texAtlas("res/block/tex/DIRT.png");

    Chunk chunk(8, 32, &shader, &texAtlas);
    Atlas atlas;
    RenderObject test(glm::vec3(0,0,0), std::vector<float>{0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1}, std::vector<uint>{0,1,2}, &shader, &texAtlas);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(app.GetWindow()))
    {
        app.processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        chunk.Draw(app);
        test.Draw(app);

        glfwSwapBuffers(app.GetWindow());
        glfwPollEvents();
    }

    glfwTerminate();
}
