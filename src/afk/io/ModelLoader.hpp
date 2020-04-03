#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "afk/renderer/Model.hpp"
#include "afk/renderer/Texture.hpp"

namespace Afk {
  class ModelLoader {
  public:
    Model model = {};

    auto load(const std::string &path) -> Model;

  private:
    static constexpr unsigned assimpOptions =
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
        aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace;

    auto processNode(const aiScene *scene, const aiNode *node, glm::mat4 transform) -> void;
    auto processMesh(const aiScene *scene, const aiMesh *mesh, glm::mat4 transform) -> Mesh;
    auto getVertices(const aiMesh *mesh) -> Mesh::Vertices;
    auto getIndices(const aiMesh *mesh) -> Mesh::Indices;
    auto getTextures(const aiMaterial *material) -> Mesh::Textures;
    auto getMaterialTextures(const aiMaterial *material, Texture::Type type)
        -> Mesh::Textures;
    auto getTexturePath(const std::string &path) const -> std::string;
  };
}
