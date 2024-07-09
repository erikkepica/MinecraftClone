#pragma once
#include<vector>
#include<unordered_map>
#include<iostream>
#include<string>
#include<array>

#include"glm/glm.hpp"

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
    Block(std::string name, bool transparent);

    glm::vec2* GetAtlasCoords(){return m_texAtlasCoord.data();}
    void SetAtlasCoord(std::array<glm::vec2,4> atlasCoord) {m_texAtlasCoord = atlasCoord;}
    bool IsTransparent(){return m_Transparent;}
    uint8_t GetID(){return m_ID;}
    std::string GetName(){return m_Name;}
private:
    bool m_Transparent;
    uint8_t m_ID;
    std::string m_Name;
    std::array<glm::vec2,4> m_texAtlasCoord; 
};

class Blocks
{
public:
    Blocks(){}
    void AddBlock(Block block){m_Blocks.push_back(block);}
    Block GetBlock(std::uint8_t id);
    Block* GetBlockPtr(std::uint8_t id);
    uint8_t GetID(std::string name);
    std::vector<Block> GetBlocks(){return m_Blocks;}
private:
    std::vector<Block> m_Blocks;
};
