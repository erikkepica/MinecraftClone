#pragma once
#include"Graphics.h"
#include<vector>
#include<algorithm>
#include<array>
#include<unordered_map>

class Face
{
public:
    enum FaceType
    {
        front,
        back,
        right,
        left,
        top,
        bottom,
        SIZE
    };

    inline const static std::vector<uint> m_Indices
    {
        0,1,3,
        1,2,3
    };
    
    inline const static std::unordered_map<FaceType, std::vector<float>> m_FaceVerticiesMap
    {
        {front,{
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f, 
        0.5f,  0.5f, 0.5f, 
        -0.5f,  0.5f, 0.5f
        }},
        {back,{
        -0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f  
        }},
        {right,{
        0.5f, -0.5f, 0.5f,  
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, 0.5f  
        }},
        {left,{
        -0.5f, -0.5f, 0.5f,  
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, 0.5f 
        }},
        {top,{
        -0.5f, 0.5f, -0.5f,  
        -0.5f,  0.5f, 0.5f,
        0.5f,  0.5f, 0.5f,
        0.5f, 0.5f, -0.5f
        }},
        {bottom,{
        -0.5f, -0.5f, -0.5f,  
        -0.5f,  -0.5f, 0.5f,
        0.5f,  -0.5f, 0.5f,
        0.5f, -0.5f, -0.5f 
        }},
    };
};

class Block
{
public:
    inline static uint8_t IDCount = 1;

    Block(){}
    Block(std::string name, std::array<glm::vec2, 4> texAtlasCoord, bool transparent);

    glm::vec2* GetAtlasCoords(){return m_texAtlasCoord;}
    bool IsTransparent(){return m_Transparent;}
    uint8_t GetID(){return m_ID;}
    std::string GetName(){return m_Name;}
private:
    bool m_Transparent;
    uint8_t m_ID;
    std::string m_Name;
    glm::vec2 m_texAtlasCoord[4]; 
};

class Blocks
{
public:
    static void AddBlock(Block block){m_Blocks.push_back(block);}
    static Block GetBlock(std::uint8_t id);
    static uint8_t GetID(std::string name);
    static std::vector<Block> GetBlocks(){return m_Blocks;}
private:
    inline static std::vector<Block> m_Blocks;
};

class Chunk
{
public:
    Chunk(int sizeXZ, int sizeY, Shader* shader, Texture* texture);
    void Bake();
    void InitRenderObject();
    void Draw(Application app);
    std::array<bool, 6> CheckNeighbours(glm::vec3 pos);
private:
    std::vector<float> m_Vertices;
    std::vector<uint> m_Indices;

    Shader* m_Shader;
    Texture* m_Texture;

    int m_SizeXZ;
    int m_SizeY;

    uint8_t* m_Blocks;

    std::unique_ptr<RenderObject> m_RenderObject;
};