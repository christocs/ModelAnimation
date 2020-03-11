#pragma once

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Mesh.hpp"
#include "Shader.hpp"

class Model {
  public:
    std::string dir               = "";
    std::vector<Mesh> meshes      = {};
    std::vector<Texture> textures = {};

    Model(std::string const &path);

    auto draw(Shader shader) const -> void;

  private:
    auto loadModel(std::string const &path) -> void;
    auto processNode(const aiNode *node, const aiScene *scene) -> void;
    auto processMesh(const aiMesh *mesh, const aiScene *scene) -> Mesh;
    auto loadMaterialTextures(const aiMaterial *mat, const aiTextureType type,
                              const std::string &typeName) -> std::vector<Texture>;
    auto loadTexture(const std::string &path) -> GLuint;
};
