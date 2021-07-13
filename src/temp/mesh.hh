#pragma once

#include <string>
#include <vector>
#include "program.hh"
#include "wrappers/glWrapper.hh"
#include "wrappers/glmWrapper.hh"

struct Vertex {
    glm::vec3 Position; // position of the vertex
    glm::vec3 Normal; // Normal of the vertex
    glm::vec2 TexCoords; // coordinates on the texture
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    // mesh data
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    std::vector<Texture> textures_;

    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned int> indices,
         std::vector<Texture> textures)
         : vertices_(vertices), indices_(indices), textures_(textures)
    {
        setupMesh();
    }

    void draw(program* program);

private:
    // render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};


