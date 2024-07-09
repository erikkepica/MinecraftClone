#include"Game.h"

int main()
{
    Game game(800, 600, "Minecraft Clone", "res/block/");
    
    while (!glfwWindowShouldClose(game.GetApplication()->GetWindow()))
    {
        game.Update();
    }

    glfwTerminate();
}
