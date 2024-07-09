#pragma once
#include "glad/glad.h"

#include<fstream>

#include"json.hpp"

#include"Applocation.h"
#include"Atlas.h"
#include"World.h"
#include"Block.h"


class Game
{
public:
    Game(int width, int height, const char* name, std::string blockResPath);
    
    void QueueBlocks();

    void RegisterBlocks();
    
    void Update();

    void QueueBlock(std::string name);

    Application* GetApplication(){return m_App.get();}
    
    Blocks* GetBlocks(){return m_Blocks.get();}
    
    World* GetWorld(){return m_World.get();}

    inline static Game* GetGame(){if(instance != nullptr) return instance;}

    Texture atlasTex;
    Shader defultShader;

    inline static Game* instance;
private:
    std::unique_ptr<World> m_World;

    std::unique_ptr<Application> m_App;
    std::unique_ptr<Atlas> m_Atlas;

    std::unique_ptr<Blocks> m_Blocks;

    std::vector<std::string> m_BlockRegisterQueue;

    std::string m_BlockResPath;

    float m_LastFrame;
    float m_DeltaTime;
};