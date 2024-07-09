#pragma once
#include"Chunk.h"
#include<memory>

class World
{
public:
    World(Application* app, Shader* shader, Texture* tex, int renderDistance)
        :m_App(app), m_RenderDistance(renderDistance)
    {
        m_Chunks = std::make_unique<std::vector<Chunk>>();
        for(int x = 0; x < m_RenderDistance*2+1; x++)
        {
            for(int z = 0; z < m_RenderDistance*2+1;z ++)
            {
                m_Chunks->push_back(Chunk(16,128, (x-m_RenderDistance)*16, (z-m_RenderDistance)*16, shader, tex));
            }
        }
    }

    Chunk* GetChunk(glm::vec2 pos)
    {
        for(int i = 0; i < m_Chunks->size();i++)
        {
            glm::vec2 startChunkPos = m_Chunks->at(i).GetPos();
            glm::vec2 endChunkPos = glm::vec2(startChunkPos.x+16,startChunkPos.y+16); 
            if(pos.x > startChunkPos.x && pos.x < endChunkPos.x && pos.y > startChunkPos.y && pos.x < endChunkPos.y)
            {
                return &m_Chunks->at(i);
            }
        }
        return nullptr;
    }

    void Update()
    {
        for(int i = 0; i < m_Chunks->size();i ++)
        {
            m_Chunks->at(i).Draw(*m_App);
        }
    }
private:
    Application* m_App;
    int m_RenderDistance;
    std::unique_ptr<std::vector<Chunk>> m_Chunks;
};