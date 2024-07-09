#include "Chunk.h"
#include"Block.h"
#include"Game.h"
#include"FastNoiseLite.h"

Chunk::Chunk(int sizeXZ, int sizeY, int posX, int posY, Shader* shader, Texture* texture)
    :m_SizeXZ(sizeXZ), m_SizeY(sizeY), m_RenderObject(nullptr), m_Shader(shader), m_Texture(texture)
{
    m_Pos = glm::vec2(posX, posY);
    m_Blocks = new uint8_t[m_SizeXZ*m_SizeXZ*m_SizeY];

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    noise.SetSeed(0);

    noise.SetFrequency(0.02f);

    noise.SetFractalType(FastNoiseLite::FractalType_FBm);

    noise.SetFractalOctaves(2);
    noise.SetFractalLacunarity(2.0f);
    noise.SetFractalGain(0.1f);
    noise.SetFractalWeightedStrength(7.0f);

    noise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_EuclideanSq);

    for(int x = 0; x < m_SizeXZ; x++)
    {
        for(int z = 0; z < m_SizeXZ; z++)
        {
            noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
            noise.SetFrequency(0.05f);
            int height1 = noise.GetNoise((float)x+m_Pos.y, (float)z+m_Pos.x)*10.0f+16.0f;
            noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
            noise.SetFrequency(0.02f);
            int height2 = noise.GetNoise((float)x+m_Pos.y, (float)z+m_Pos.x)*10.0f+16.0f;
            /*noise.SetFrequency(0.01f);
            noise.SetFractalType(FastNoiseLite::FractalType_None);
            int height2 = noise.GetNoise((float)x+m_Pos.x, (float)z+m_Pos.y)*5.0f+70.0f;*/
            int height = height1+height2;
            for(int y = 0; y < m_SizeY; y++)
            {
                uint8_t ID = Game::GetGame()->GetBlocks()->GetID("STONE");
                if(y>height-5) ID = Game::GetGame()->GetBlocks()->GetID("DIRT");
                if(y==height) ID = Game::GetGame()->GetBlocks()->GetID("GRASS");
                if(y>height) ID = 0;
                m_Blocks[y*m_SizeXZ*m_SizeXZ+x*m_SizeXZ+z] = ID;    
            }
        }
    }
    Bake();
}

void Chunk::Bake()
{
    for(int y = 0; y < m_SizeY; y++)
    {
        for(int x = 0; x < m_SizeXZ; x++)
        {
            for(int z = 0; z < m_SizeXZ; z++)
            {
                if(m_Blocks[y*m_SizeXZ*m_SizeXZ+x*m_SizeXZ+z] == 0) continue;
                std::array<bool, Face::SIZE> noNeighbours = CheckNeighbours(glm::vec3(x,y,z));
                for(int f = 0; f <Face::SIZE; f++)
                {
                    if(noNeighbours[f])
                    {
                        for(int i = 0; i < Face::m_Indices.size(); i++)
                        {
                            m_Indices.push_back(Face::m_Indices[i]+m_Vertices.size()/5);
                        }
                        for(int i = 0; i < 4; i++)
                        {
                            m_Vertices.push_back(Face::m_FaceVerticiesMap.at(static_cast<Face::FaceType>(f))[i*3]+x);
                            m_Vertices.push_back(Face::m_FaceVerticiesMap.at(static_cast<Face::FaceType>(f))[i*3+1]+y);
                            m_Vertices.push_back(Face::m_FaceVerticiesMap.at(static_cast<Face::FaceType>(f))[i*3+2]+z);

                            m_Vertices.push_back(Game::GetGame()->GetBlocks()->GetBlock(m_Blocks[y*m_SizeXZ*m_SizeXZ + x*m_SizeXZ + z]).GetAtlasCoords()[i].x);
                            m_Vertices.push_back(Game::GetGame()->GetBlocks()->GetBlock(m_Blocks[y*m_SizeXZ*m_SizeXZ + x*m_SizeXZ + z]).GetAtlasCoords()[i].y);
                        }
                    }
                }
            }
        }
    }
    InitRenderObject();
}

void Chunk::InitRenderObject()
{
    m_RenderObject = std::make_unique<RenderObject>(glm::vec3(m_Pos.y,0,m_Pos.x), m_Vertices, m_Indices, m_Shader, m_Texture);
}

void Chunk::Draw(Application app)
{
    m_RenderObject->Draw(app);
}

glm::vec3 Chunk::GetBlock(glm::vec3 pos)
{
    return glm::vec3(std::floor(pos.x-m_Pos.x),std::floor(pos.y),std::floor(pos.z-m_Pos.y));
}

std::array<bool, 6> Chunk::CheckNeighbours(glm::vec3 pos)
{
    std::array<bool, Face::SIZE> noNeighbours;
    
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    std::fill(noNeighbours.begin(), noNeighbours.end(), false);


    if(pos.x == m_SizeXZ-1) noNeighbours[Face::right] = true;
    else if(m_Blocks[y*m_SizeXZ*m_SizeXZ+(x+1)*m_SizeXZ+z] == 0|| Game::GetGame()->GetBlocks()->GetBlock(m_Blocks[y*m_SizeXZ*m_SizeXZ+(x+1)*m_SizeXZ+z]).IsTransparent()) noNeighbours[Face::right] = true;
    if(pos.x == 0) noNeighbours[Face::left] = true;
    else if(m_Blocks[y*m_SizeXZ*m_SizeXZ+(x-1)*m_SizeXZ+z] == 0|| Game::GetGame()->GetBlocks()->GetBlock(m_Blocks[y*m_SizeXZ*m_SizeXZ+(x-1)*m_SizeXZ+z]).IsTransparent()) noNeighbours[Face::left] = true;
    if(pos.z == m_SizeXZ-1) noNeighbours[Face::front] = true;
    else if(m_Blocks[y*m_SizeXZ*m_SizeXZ+x*m_SizeXZ+(z+1)] == 0|| Game::GetGame()->GetBlocks()->GetBlock(m_Blocks[y*m_SizeXZ*m_SizeXZ+x*m_SizeXZ+(z+1)]).IsTransparent()) noNeighbours[Face::front] = true;
    if(pos.z == 0) noNeighbours[Face::back] = true;
    else if(m_Blocks[y*m_SizeXZ*m_SizeXZ+x*m_SizeXZ+(z-1)] == 0|| Game::GetGame()->GetBlocks()->GetBlock(m_Blocks[y*m_SizeXZ*m_SizeXZ+x*m_SizeXZ+(z-1)]).IsTransparent()) noNeighbours[Face::back] = true;
    if(pos.y == m_SizeY-1) noNeighbours[Face::top] = true;
    else if(m_Blocks[(y+1)*m_SizeXZ*m_SizeXZ+x*m_SizeXZ+z] == 0|| Game::GetGame()->GetBlocks()->GetBlock(m_Blocks[(y+1)*m_SizeXZ*m_SizeXZ+x*m_SizeXZ+z]).IsTransparent()) noNeighbours[Face::top] = true;
    if(pos.y == 0) noNeighbours[Face::bottom] = true;
    else if(m_Blocks[(y-1)*m_SizeXZ*m_SizeXZ+x*m_SizeXZ+z] == 0|| Game::GetGame()->GetBlocks()->GetBlock(m_Blocks[(y-1)*m_SizeXZ*m_SizeXZ+x*m_SizeXZ+z]).IsTransparent()) noNeighbours[Face::bottom] = true;



    
    
    
    


    return noNeighbours;
}

uint8_t Chunk::GetBlockID(glm::vec3 pos)
{
    return m_Blocks[PosToIndex(pos)];
}

int Chunk::PosToIndex(glm::vec3 pos)
{
    return pos.y*m_SizeXZ*m_SizeXZ+pos.x*m_SizeXZ+pos.z;
}
