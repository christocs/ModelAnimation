#include "afk/io/ModelLoader.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cpplocate/cpplocate.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "afk/io/Path.hpp"
#include "afk/renderer/Mesh.hpp"
#include "afk/renderer/Model.hpp"
#include "afk/renderer/Texture.hpp"

using namespace std::string_literals;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using std::runtime_error;
using std::string;
using std::unordered_map;
using std::vector;

using Afk::ModelLoader;
using Afk::Path;
using Afk::Texture;

static auto getAssimpTextureType(Texture::Type type) -> aiTextureType {
  static const auto types = unordered_map<Texture::Type, aiTextureType>{
      {Texture::Type::Diffuse, aiTextureType_DIFFUSE},
      {Texture::Type::Specular, aiTextureType_SPECULAR},
      {Texture::Type::Normal, aiTextureType_NORMALS},
      {Texture::Type::Height, aiTextureType_HEIGHT},
  };

  return types.at(type);
}

static auto toGlm(aiMatrix4x4t<float> m) -> mat4 {
  return mat4{m.a1, m.b1, m.c1, m.d1,  //
              m.a2, m.b2, m.c2, m.d2,  //
              m.a3, m.b3, m.c3, m.d3,  //
              m.a4, m.b4, m.c4, m.d4}; //
}

static auto toGlm(aiVector3t<float> m) -> vec3 {
  return vec3{m.x, m.y, m.z};
}

auto ModelLoader::load(const string &path) -> Model {
  const auto absPath = Path::getAbsolutePath(path);
  auto importer      = Assimp::Importer{};

  this->model.path = path;
  this->model.dir  = Path::getDirectory(path);

  const auto *scene = importer.ReadFile(absPath, this->assimpOptions);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    throw runtime_error{"Model load error: "s + importer.GetErrorString()};
  }

  this->model.meshes.reserve(scene->mNumMeshes);
  this->processNode(scene, scene->mRootNode, toGlm(scene->mRootNode->mTransformation));

  return std::move(this->model);
}

auto ModelLoader::processNode(const aiScene *scene, const aiNode *node,
                              glm::mat4 transform) -> void {
  // Process all meshes at this node.
  for (auto i = size_t{0}; i < node->mNumMeshes; ++i) {
    const auto *mesh = scene->mMeshes[node->mMeshes[i]];

    this->model.meshes.push_back(
        this->processMesh(scene, mesh, transform * toGlm(node->mTransformation)));
  }

  // Process all child nodes.
  for (auto i = size_t{0}; i < node->mNumChildren; ++i) {
    this->processNode(scene, node->mChildren[i], transform * toGlm(node->mTransformation));
  }
}

auto ModelLoader::processMesh(const aiScene *scene, const aiMesh *mesh, mat4 transform)
    -> Mesh {
  auto newMesh = Mesh{};

  newMesh.vertices = this->getVertices(mesh);
  newMesh.indices  = this->getIndices(mesh);
  newMesh.textures = this->getTextures(scene->mMaterials[mesh->mMaterialIndex]);
  newMesh.transform = transform;

  return newMesh;
}

auto ModelLoader::getVertices(const aiMesh *mesh) -> Mesh::Vertices {
  auto vertices     = Mesh::Vertices{};
  const auto hasUvs = mesh->HasTextureCoords(0);

  vertices.reserve(mesh->mNumVertices);

  for (auto i = size_t{0}; i < mesh->mNumVertices; ++i) {
    auto vertex = Vertex{};

    vertex.position = toGlm(mesh->mVertices[i]);
    vertex.normal   = toGlm(mesh->mNormals[i]);

    if (hasUvs) {
      vertex.uvs       = toGlm(mesh->mTextureCoords[0][i]);
      vertex.tangent   = toGlm(mesh->mTangents[i]);
      vertex.bitangent = toGlm(mesh->mBitangents[i]);
    }

    vertices.push_back(std::move(vertex));
  }

  return vertices;
}

auto ModelLoader::getIndices(const aiMesh *mesh) -> Mesh::Indices {
  auto indices = Mesh::Indices{};

  indices.reserve(mesh->mNumFaces);

  for (auto i = size_t{0}; i < mesh->mNumFaces; ++i) {
    const auto face = mesh->mFaces[i];

    for (auto j = size_t{0}; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }

  return indices;
}

auto ModelLoader::getMaterialTextures(const aiMaterial *material, Texture::Type type)
    -> Mesh::Textures {
  auto textures = Mesh::Textures{};

  const auto textureCount = material->GetTextureCount(getAssimpTextureType(type));

  textures.reserve(textureCount);

  for (auto i = 0u; i < textureCount; ++i) {
    auto assimpPath = aiString{};
    material->GetTexture(getAssimpTextureType(type), i, &assimpPath);
    const auto path = getTexturePath(string{assimpPath.C_Str()});

    auto texture = Texture{};
    texture.type = type;
    texture.path = path;
    textures.push_back(texture);
  }

  return textures;
}

auto ModelLoader::getTextures(const aiMaterial *material) -> Mesh::Textures {
  auto textures = Mesh::Textures{};

  const auto diffuse = this->getMaterialTextures(material, Texture::Type::Diffuse);
  const auto specular = this->getMaterialTextures(material, Texture::Type::Specular);
  const auto normal = this->getMaterialTextures(material, Texture::Type::Normal);
  const auto height = this->getMaterialTextures(material, Texture::Type::Height);

  textures.insert(textures.end(), std::make_move_iterator(diffuse.begin()),
                  std::make_move_iterator(diffuse.end()));
  textures.insert(textures.end(), std::make_move_iterator(specular.begin()),
                  std::make_move_iterator(specular.end()));
  textures.insert(textures.end(), std::make_move_iterator(normal.begin()),
                  std::make_move_iterator(normal.end()));
  textures.insert(textures.end(), std::make_move_iterator(height.begin()),
                  std::make_move_iterator(height.end()));

  return textures;
}

auto ModelLoader::getTexturePath(const string &path) const -> string {
  return this->model.dir + "/textures/" + Path::getFilename(path);
}
