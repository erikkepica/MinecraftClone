#pragma once
#include<vector>
#include<string>
#include"json.hpp"
#include"stb_image.h"
#include<fstream>
#include"Chunk.h"
#include<cmath>
#include"glm/vec2.hpp"
#include<array>

using json = nlohmann::json;


class AtlasCoords
{
    std::array<glm::vec2,4> coords;

    enum TexWrapType
    {
        CUBE_ALL,
        CUBE_GRASS_LIKE,
        CUBE_LOG_LIKE
    };

    AtlasCoords(TexWrapType type, const char* name, std::array<glm::vec2, 4> coords)
        :m_Type(type),m_Name(name), coords(coords)
    {
    };


private:
    TexWrapType m_Type;
    std::string m_Name;
};

class Atlas
{
public:
    Atlas()
    {
        std::vector<std::string> paths;

        for(auto block : Blocks::GetBlocks())
        {
            std::ifstream stream("res/block/def/"+block.GetName()+".json");
            if(!stream)
            {
                std::cerr << "Found no json in res/block/def/ for " + block.GetName() << std::endl;
            }
            json js = json::parse(stream);
            std::string type = js["type"];

            if(type == "CUBE_ALL")
            {
                paths.push_back(block.GetName());
            }
            else if(type == "CUBE_GRASS_LIKE")
            {
                paths.push_back(block.GetName()+"_TOP");
                paths.push_back(block.GetName()+"_SIDE");
                paths.push_back(block.GetName()+"_BOTTOM");
            }
            else if(type == "CUBE_LOG_LIKE")
            {
                paths.push_back(block.GetName()+"_TOP_BOTTOM");
                paths.push_back(block.GetName()+"_SIDE");
            }
        }
        int m_TexCount = paths.size();
        int m_Size = std::ceil(sqrt(m_TexCount));

        unsigned char image[m_Size * m_Size * 4 * 256];

        int row = 0;

        for(int i = 0; i < m_TexCount; i++)
        {
            int width, height, channels;
            const char* path = ("res/block/tex/"+paths[i]+".png").c_str();
            unsigned char* img = stbi_load(path, &width, &height, &channels, 4);
            if (img == nullptr)
            {
                std::cerr << "Failed to load image: " << paths[i] << std::endl;                
            }
        }
    }

private:
    int m_Size;
    int m_TexCount;
    std::vector<AtlasCoords> m_Coords;
};