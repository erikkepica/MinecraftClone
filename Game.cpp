#include "Game.h"

using json = nlohmann::json;

Game::Game(int width, int height, const char *name, std::string blockResPath)
:m_BlockResPath(blockResPath)
{
    instance = this;
    m_App = std::make_unique<Application>(width,height,name);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }
    
    m_Blocks = std::make_unique<Blocks>();
    QueueBlocks();
    RegisterBlocks();

    m_Atlas = std::make_unique<Atlas>(16, 4, blockResPath+"atlas.png");
    atlasTex = Texture((blockResPath+"atlas.png").c_str());

    defultShader = Shader("shaders/vert.glsl", "shaders/frag.glsl");
    
    m_World = std::make_unique<World>(m_App.get(), &defultShader, &atlasTex, 5);


    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
}

void Game::QueueBlocks()
{
    QueueBlock("GRASS");
    QueueBlock("DIRT");
    QueueBlock("STONE");
    QueueBlock("GLASS");
}

void Game::RegisterBlocks()
{
    for(int i = 0; i < m_BlockRegisterQueue.size(); i++)
    {
        std::ifstream stream(m_BlockResPath+"def/"+m_BlockRegisterQueue[i]+".json");
        json js = json::parse(stream);

        bool transparent = js["transparent"];

        m_Blocks->AddBlock(Block(m_BlockRegisterQueue[i], transparent));
    }
    m_BlockRegisterQueue.clear();
}

void Game::Update()
{
    float currentFrame = glfwGetTime();
    m_DeltaTime = currentFrame - m_LastFrame;
    m_LastFrame = currentFrame;

    m_App->deltaTime = m_DeltaTime;

    m_App->processInput();

    glClearColor(95.0f/255.0f, 205.0f/255.0f, 228.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_World->Update();

    glfwSwapBuffers(m_App->GetWindow());
    glfwPollEvents();
}

void Game::QueueBlock(std::string name)
{
    m_BlockRegisterQueue.push_back(name);
}
