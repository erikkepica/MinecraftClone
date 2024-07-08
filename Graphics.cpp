#include"Graphics.h"

VertexArrayObject::VertexArrayObject()
{
    glGenVertexArrays(1, &m_ID);
}
void VertexArrayObject::DoAttribs()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}


VertexBufferObject::VertexBufferObject(std::vector<float> vertices)
{
    m_Vertices = vertices;
    glGenBuffers(1, &m_ID);
}
void VertexBufferObject::FillData()
{
    float *verticesArr = m_Vertices.data();
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size()*sizeof(decltype(m_Vertices)::value_type), verticesArr, GL_STATIC_DRAW);
}


ElementBufferObject::ElementBufferObject(std::vector<unsigned int> indices)
{
    m_Indices = indices;
    glGenBuffers(1, &m_ID);
}
void ElementBufferObject::FillData()
{
    unsigned int* indicesArr = m_Indices.data();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size()*sizeof(decltype(m_Indices)::value_type), indicesArr, GL_STATIC_DRAW);
}


Texture::Texture(const char* path)
{
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 4);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
}

void Texture::Use()
{
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

RenderObject::~RenderObject()
{
}

void RenderObject::Draw(Application app)
{
    glm::mat4 trans = glm::mat4(1.0f);

    trans = glm::translate(trans, position);

    trans = glm::rotate(trans, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
    trans = glm::rotate(trans, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
    trans = glm::rotate(trans, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));

    trans = glm::scale(trans, scale);

    m_Shader->use();

    m_Shader->setMat4("model", trans);
    m_Shader->setMat4("view", app.GetCam().GetViewMat());
    m_Shader->setMat4("projection", app.GetCam().GetProjectionMat());

    m_Tex->Use();
    m_VAO.bind();
    m_EBO.Bind(); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawElements(GL_TRIANGLES, m_EBO.GetIndicesCount(), GL_UNSIGNED_INT, 0);
}