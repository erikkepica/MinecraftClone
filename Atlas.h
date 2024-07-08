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
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"stb_image_write.h"


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
    Atlas(int sizeOfImgs, int channels_, const char* outputPath)
    {
        std::vector<std::string> paths;

        for (auto block : Blocks::GetBlocks()) {
            m_Coords.push_back(AtlasCoords(block.GetName().c_str()));
            std::ifstream stream("res/block/def/" + block.GetName() + ".json");
            if (!stream) {
                std::cerr << "Found no json in res/block/def/ for " + block.GetName() << std::endl;
            }
            json js = json::parse(stream);
            std::string type = js["type"];

            if (type == "CUBE_ALL") {
                paths.push_back(block.GetName());
                m_Coords[m_Coords.size() - 1].type = AtlasCoords::CUBE_ALL;
            } else if (type == "CUBE_GRASS_LIKE") {
                paths.push_back(block.GetName() + "_TOP");
                paths.push_back(block.GetName() + "_SIDE");
                paths.push_back(block.GetName() + "_BOTTOM");
                m_Coords[m_Coords.size() - 1].type = AtlasCoords::CUBE_GRASS_LIKE;
            } else if (type == "CUBE_LOG_LIKE") {
                paths.push_back(block.GetName() + "_TOP_BOTTOM");
                paths.push_back(block.GetName() + "_SIDE");
                m_Coords[m_Coords.size() - 1].type = AtlasCoords::CUBE_LOG_LIKE;
            }
        }

        int m_TexCount = paths.size();
        int m_Size = std::ceil(std::sqrt(m_TexCount));
        if(m_Size%2!=0)
        {
            m_Size++;
        }
        int atlasWidth = m_Size * sizeOfImgs;
        int atlasHeight = m_Size * sizeOfImgs;
        std::vector<unsigned char> image(atlasWidth * atlasHeight * channels_, 0);

        int indexOnAtlas = 0;
        for (int i = 0; i < m_Coords.size(); i++) {
            int size = 0;
            switch (m_Coords[i].type) {
                case AtlasCoords::CUBE_ALL:
                    size = 1;
                    break;
                case AtlasCoords::CUBE_GRASS_LIKE:
                    size = 3;
                    break;
                case AtlasCoords::CUBE_LOG_LIKE:
                    size = 2;
                    break;
            }
            for (int j = 0; j < size; j++, indexOnAtlas++) {
                int row = indexOnAtlas / m_Size;
                int col = indexOnAtlas % m_Size;
                m_Coords[i].AddCoords(std::array<glm::vec2, 4>{
                    glm::vec2((float)col / (float)m_Size, (float)(row + 1) / (float)m_Size),
                    glm::vec2((float)(col + 1) / (float)m_Size, (float)(row + 1) / (float)m_Size),
                    glm::vec2((float)(col + 1) / (float)m_Size, (float)row / (float)m_Size),
                    glm::vec2((float)col / (float)m_Size, (float)row / (float)m_Size),
                });
                Blocks::GetBlockPtr(Blocks::GetID(m_Coords[i].GetName()))->SetAtlasCoord(m_Coords[i].GetCoords(0));
            }
        }

        for (int i = 0; i < m_TexCount; i++) {
            int width, height, channels;
            std::string path = "res/block/tex/" + paths[i] + ".png";
            std::cout << path << std::endl;
            unsigned char* img = stbi_load(path.c_str(), &width, &height, &channels, channels_);
            if (img == nullptr) {
                std::cerr << "Failed to load image: " << path << std::endl;
                continue;
            }

            int row = i / m_Size;
            int col = i % m_Size;

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    for (int c = 0; c < channels_; c++) {
                        int atlasIndex = ((row * sizeOfImgs + y) * atlasWidth + (col * sizeOfImgs + x)) * channels_ + c;
                        int imgIndex = (y * width + x) * channels_ + c;
                        image[atlasIndex] = img[imgIndex];
                    }
                }
            }
            stbi_image_free(img);
        }

        if (stbi_write_png(outputPath, atlasWidth, atlasHeight, channels_, image.data(), atlasWidth * channels_)) {
            std::cout << "Image saved successfully as output.png" << std::endl;
        } else {
            std::cerr << "Failed to save image" << std::endl;
        }
    /*    std::vector<std::string> paths;

        for(auto block : Blocks::GetBlocks())
        {
            m_Coords.push_back(AtlasCoords(block.GetName().c_str()));
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
                m_Coords[m_Coords.size()-1].type = AtlasCoords::CUBE_ALL;
            }
            else if(type == "CUBE_GRASS_LIKE")
            {
                paths.push_back(block.GetName()+"_TOP");
                paths.push_back(block.GetName()+"_SIDE");
                paths.push_back(block.GetName()+"_BOTTOM");
                m_Coords[m_Coords.size()-1].type = AtlasCoords::CUBE_GRASS_LIKE;
            }
            else if(type == "CUBE_LOG_LIKE")
            {
                paths.push_back(block.GetName()+"_TOP_BOTTOM");
                paths.push_back(block.GetName()+"_SIDE");
                m_Coords[m_Coords.size()-1].type = AtlasCoords::CUBE_LOG_LIKE;
            }
        }
        int m_TexCount = paths.size();
        int m_Size = std::ceil(std::sqrt(m_TexCount));
        int atlasWidth = m_Size * sizeOfImgs;
        int atlasHeight = m_Size * sizeOfImgs;
        std::vector<unsigned char> image(atlasWidth * atlasHeight * channels_, 0);

        int indexOnAtlas = 0;
        for(int i = 0; i < m_Coords.size(); i++)
        {
            int size = 0;
            switch(m_Coords[i].type)
            {
                case AtlasCoords::CUBE_ALL:
                    size = 1;
                    break;
                case AtlasCoords::CUBE_GRASS_LIKE:
                    size = 3;
                    break;
                case AtlasCoords::CUBE_LOG_LIKE:
                    size = 2;
                    break;
            }
            int startIndexOnAtlas = indexOnAtlas;
            for(indexOnAtlas; indexOnAtlas <= startIndexOnAtlas+size; indexOnAtlas++)
            {
                int row = indexOnAtlas / m_Size;
                int col = indexOnAtlas % m_Size;
                std::cout << (m_Coords[i].GetName()) << std::endl;
                m_Coords[i].AddCoords(std::array<glm::vec2, 4>{
                    glm::vec2((float)col/(float)m_Size, (float)(m_Size-(row+1))/(float)m_Size),
                    glm::vec2((float)(col+1)/(float)m_Size, (float)(m_Size-(row+1))/(float)m_Size),
                    glm::vec2((float)(col+1)/(float)m_Size, (float)(m_Size-row)/(float)m_Size),
                    glm::vec2((float)col/(float)m_Size, (float)(m_Size-row)/(float)m_Size),
                });
                Blocks::GetBlockPtr(Blocks::GetID(m_Coords[i].GetName()))->SetAtlasCoord(m_Coords[i].GetCoords(0));
            }
        }

        for(int i = 0; i < m_TexCount; i++) {
            int width, height, channels;
            std::string path = "res/block/tex/" + paths[i] + ".png";
            std::cout << path << std::endl;
            unsigned char* img = stbi_load(path.c_str(), &width, &height, &channels, channels_);
            if (img == nullptr) {
                std::cerr << "Failed to load image: " << path << std::endl;
                continue;
            }

            int row = i / m_Size;
            int col = i % m_Size;

            for(int y = 0; y < height; y++) {
                for(int x = 0; x < width; x++) {
                    for(int c = 0; c < channels_; c++) {
                        int atlasIndex = ((row * sizeOfImgs + y) * atlasWidth + (col * sizeOfImgs + x)) * channels_ + c;
                        int imgIndex = (y * width + x) * channels_ + c;
                        image[atlasIndex] = img[imgIndex];
                    }
                }
            }
            stbi_image_free(img);
        }

        if (stbi_write_png(outputPath, atlasWidth, atlasHeight, channels_, image.data(), atlasWidth * channels_)) {
            std::cout << "Image saved successfully as output.png" << std::endl;
        } else {
            std::cerr << "Failed to save image" << std::endl;
        }        */
    }
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