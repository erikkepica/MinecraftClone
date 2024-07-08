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

    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    
    Blocks::AddBlock(Block("GRASS", false));
    Blocks::AddBlock(Block("STONE", false));
    Blocks::AddBlock(Block("DIRT", false));
    Blocks::AddBlock(Block("GLASS", true));

    Atlas atlas(16, 4, "res/atlas.png");

    Shader shader("shaders/vert.glsl", "shaders/frag.glsl");
    Texture texAtlas("res/atlas.png");

    Chunk chunk(8, 128, &shader, &texAtlas);

    float lastFrame;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(app.GetWindow()))
    {
        float currentFrame = glfwGetTime();
        app.deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        app.processInput();

        std::cout << 1/app.deltaTime << std::endl;

        glClearColor(95.0f/255.0f, 205.0f/255.0f, 228.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        chunk.Draw(app);

        glfwSwapBuffers(app.GetWindow());
        glfwPollEvents();
    }

    glfwTerminate();
}
