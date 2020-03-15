#pragma once

#include <string>
#include <vector>

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Model.hpp"

class ModelLoader {
  public:
    using Textures = std::vector<Texture>;

    static constexpr unsigned ASSIMP_PROCESSING_FLAGS =
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
        aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace;

    auto load(const std::string &path) -> Model;

  private:
    Model model = {};

    auto processNode(const aiScene *scene, const aiNode *node,
                     glm::mat4 transform) -> void;
    auto processMesh(const aiScene *scene, const aiMesh *mesh,
                     glm::mat4 transform) -> Mesh;
    auto loadMaterialTextures(const aiMaterial *mat, const aiTextureType type,
                              const std::string &typeName) -> Textures;
    auto loadTexture(const std::string &path) -> GLuint;

    auto assimpToGlmMat4(aiMatrix4x4t<float> m) const -> glm::mat4;
    auto getModelPath(const std::string &path) const -> std::string;
    auto getModelDir(const std::string &path) const -> std::string;
    auto getTexturePath(const std::string &path) const -> std::string;
};
