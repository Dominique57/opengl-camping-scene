#pragma once

//#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <temp/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.hh"
#include "temp/program.hh"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model
{
public:
    Model(const char *path, const program& program, const glm::vec3 &textureCoef);

    void draw();

    const program& getProgram() { return program_; }

private:
    // model data
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    const program &program_;
    const glm::vec3 textureCoef_;

    void loadModel(const std::string& path);

    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial *mat,
                                              aiTextureType type,
                                              const std::string& typeName);
};

unsigned int TextureFromFile(const char *path,
                             const std::string &directory,
                             bool gamma = false);

