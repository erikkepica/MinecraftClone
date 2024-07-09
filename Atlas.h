#pragma once
#include<vector>
#include<string>
#include"json.hpp"
#include"stb_image.h"
#include<fstream>
#include<cmath>
#include"glm/vec2.hpp"
#include<array>


#include"Chunk.h"

using json = nlohmann::json;


class AtlasCoords
{
public:
    enum TexWrapType
    {
        CUBE_ALL,
        CUBE_GRASS_LIKE,
        CUBE_LOG_LIKE
    };

    AtlasCoords(const char* name)
        :m_Name(name)
    {
    };

    void AddCoords(std::array<glm::vec2, 4> coord){m_Coords.push_back(coord);}
    std::array<glm::vec2,4> GetCoords(int i) {return m_Coords[i]; }

    std::string GetName(){return m_Name;}

    TexWrapType type;
private:
    std::string m_Name;
    std::vector<std::array<glm::vec2,4>> m_Coords;
};

class Atlas
{
public:
    Atlas(int sizeOfImgs, int channels_, std::string outputPath);
    AtlasCoords GetCoords(const char* name)
    {
        int dixr = m_Coords.size();
        for(int i = 0; i < m_Coords.size(); i++)
        {
            if(m_Coords[i].GetName().c_str() == name)
            {
                return m_Coords[i];
            }
        }
        return NULL;
    }

private:
    int m_Size;
    int m_TexCount;
    std::vector<AtlasCoords> m_Coords;
};