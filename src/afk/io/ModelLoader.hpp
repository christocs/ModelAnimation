#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "afk/render/ModelData.hpp"
#include "afk/render/TextureData.hpp"

namespace Afk {
  class ModelLoader {
  public:
    ModelData model = {};

    auto load(const std::string &path) -> ModelData;

  private:
    static constexpr unsigned assimpOptions =
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
        aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace;

    auto processNode(const aiScene *scene, const aiNode *node, glm::mat4 transform) -> void;
    auto processMesh(const aiScene *scene, const aiMesh *mesh, glm::mat4 transform) -> MeshData;
    auto getVertices(const aiMesh *mesh) -> MeshData::Vertices;
    auto getIndices(const aiMesh *mesh) -> MeshData::Indices;
    auto getTextures(const aiMaterial *material) -> MeshData::Textures;
    auto getMaterialTextures(const aiMaterial *material, TextureData::Type type)
        -> MeshData::Textures;
    auto getTexturePath(const std::string &path) const -> std::string;
  };
}
