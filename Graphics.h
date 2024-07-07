#pragma once
#include"glad/glad.h"
#include"stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<vector>

#include"Shader.h"
#include"Applocation.h"

class VertexArrayObject
{
public:
    VertexArrayObject();
    
    void DoAttribs();

    void bind(){glBindVertexArray(m_ID);}
    static void unbind(){glBindVertexArray(0);}

    ~VertexArrayObject(){glDeleteVertexArrays(1,&m_ID);}
private:
    unsigned int m_ID;
};

class VertexBufferObject
{
public:
    VertexBufferObject(){}
    VertexBufferObject(std::vector<float> vertices);

    void FillData();

    void Bind(){glBindBuffer(GL_ARRAY_BUFFER, m_ID);}
    static void UnBind(){glBindBuffer(GL_ARRAY_BUFFER,0);}

    ~VertexBufferObject(){glDeleteBuffers(1, &m_ID);}
private:
    unsigned int m_ID;

    std::vector<float> m_Vertices;
};

class ElementBufferObject
{
public:
    ElementBufferObject(){}
    ElementBufferObject(std::vector<unsigned int> indices);

    void FillData();

    void Bind(){glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);}
    static void UnBind(){glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);}

    ~ElementBufferObject(){glDeleteBuffers(1, &m_ID);}
    int GetIndicesCount(){return m_Indices.size();}
private:
    unsigned int m_ID;

    std::vector<unsigned int> m_Indices;
};



class Texture
{
public:
    Texture(){}
    Texture(const char* path);

    void Use();
private:
    unsigned int m_ID;
};

class RenderObject
{
public:
    RenderObject(){}
    RenderObject(glm::vec3 pos, std::vector<float> vertices, std::vector<unsigned int> indices, Shader* shader, Texture* tex)
        : position(pos), m_VAO(), m_VBO(VertexBufferObject(vertices)), m_EBO(ElementBufferObject(indices)), m_Shader(shader),m_Tex(tex)
    {
        scale = glm::vec3(1.f,1.f,1.f);
        rotation = glm::vec3(0,0,0);

        Init();
    }

    void Init()
    {
        m_VAO.bind();

        m_EBO.Bind();
        m_EBO.FillData();

        m_VBO.Bind();
        m_VBO.FillData();

        m_VAO.DoAttribs();

        m_VAO.unbind();
        m_VBO.UnBind();
        m_EBO.UnBind;
    }

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    ~RenderObject();
    
    void Draw(Application app);
private:
    VertexArrayObject m_VAO;
    VertexBufferObject m_VBO;
    ElementBufferObject m_EBO;
    Texture* m_Tex;
    Shader* m_Shader;
};