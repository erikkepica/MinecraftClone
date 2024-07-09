#include"Block.h"
Block::Block(std::string name, bool transparent)
    : m_ID(IDCount++), m_Name(name), m_Transparent(transparent)
{
    for (int i = 0; i < 4; ++i) {
        m_texAtlasCoord[i] = glm::vec2(0,0);
    }
}

Block Blocks::GetBlock(std::uint8_t id)
{
    for(auto i : m_Blocks)
    {
        if(i.GetID() == id)
        {
            return i;
        }
    }
    throw("No block has this id!");
}

Block* Blocks::GetBlockPtr(std::uint8_t id)
{
    for(int i = 0; i < m_Blocks.size(); i++)
    {
        if(m_Blocks[i].GetID() == id)
        {
            return &m_Blocks[i];
        }
    }
    throw("No block has this id!");
}

uint8_t Blocks::GetID(std::string name)
{
    for(auto i : m_Blocks)
    {
        if(i.GetName() == name)
        {
            return i.GetID();
        }
    }
    throw("No block has this name!");
}

