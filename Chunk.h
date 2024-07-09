#pragma once
#include"Graphics.h"
#include<algorithm>
#include<array>


class Chunk
{
public:
    Chunk(int sizeXZ, int sizeY, int posX, int posY,  Shader* shader, Texture* texture);
    void Bake();
    void InitRenderObject();
    void Draw(Application app);
    glm::vec3 GetBlock(glm::vec3 pos);
    glm::vec2 GetPos(){return m_Pos;}
    std::array<bool, 6> CheckNeighbours(glm::vec3 pos);

    uint8_t GetBlockID(glm::vec3 pos);
    int PosToIndex(glm::vec3 pos);
private:
    std::vector<float> m_Vertices;
    std::vector<uint> m_Indices;

    Shader* m_Shader;
    Texture* m_Texture;

    int m_SizeXZ;
    int m_SizeY;

    glm::vec2 m_Pos;

    uint8_t* m_Blocks;

    std::unique_ptr<RenderObject> m_RenderObject;
};