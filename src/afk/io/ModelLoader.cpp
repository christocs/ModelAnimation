#include "afk/io/ModelLoader.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
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

#include "afk/io/Log.hpp"
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
using std::filesystem::path;

using Afk::ModelLoader;
using Afk::Texture;

constexpr unsigned ASSIMP_OPTIONS =
    aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
    aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_GlobalScale;

static auto get_assimp_texture_type(Texture::Type type) -> aiTextureType {
  static const auto types = unordered_map<Texture::Type, aiTextureType>{
      {Texture::Type::Diffuse, aiTextureType_DIFFUSE},
      {Texture::Type::Specular, aiTextureType_SPECULAR},
      {Texture::Type::Normal, aiTextureType_NORMALS},
      {Texture::Type::Height, aiTextureType_HEIGHT},
  };

  return types.at(type);
}

static auto to_glm(aiMatrix4x4t<float> m) -> mat4 {
  return mat4{m.a1, m.b1, m.c1, m.d1,  //
              m.a2, m.b2, m.c2, m.d2,  //
              m.a3, m.b3, m.c3, m.d3,  //
              m.a4, m.b4, m.c4, m.d4}; //
}

static auto to_glm(aiVector3t<float> m) -> vec3 {
  return vec3{m.x, m.y, m.z};
}

auto ModelLoader::load(path file_path) -> Model {
  const auto abs_path = Afk::get_resource_path(file_path);
  auto importer       = Assimp::Importer{};

  this->model.file_path = file_path;
  this->model.file_dir  = file_path.parent_path();

  Afk::status << file_path.string() << '\n';
  Afk::status << abs_path.string() << '\n';

  const auto *scene = importer.ReadFile(abs_path.string(), ASSIMP_OPTIONS);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    throw runtime_error{"Model load error: "s + importer.GetErrorString()};
  }

  this->model.meshes.reserve(scene->mNumMeshes);
  this->process_node(scene, scene->mRootNode, to_glm(scene->mRootNode->mTransformation));

  return std::move(this->model);
}

auto ModelLoader::process_node(const aiScene *scene, const aiNode *node,
                               glm::mat4 transform) -> void {
  // Process all meshes at this node.
  for (auto i = size_t{0}; i < node->mNumMeshes; ++i) {
    const auto *mesh = scene->mMeshes[node->mMeshes[i]];

    this->model.meshes.push_back(
        this->process_mesh(scene, mesh, transform * to_glm(node->mTransformation)));
  }

  // Process all child nodes.
  for (auto i = size_t{0}; i < node->mNumChildren; ++i) {
    this->process_node(scene, node->mChildren[i], transform * to_glm(node->mTransformation));
  }
}

auto ModelLoader::process_mesh(const aiScene *scene, const aiMesh *mesh, mat4 transform)
    -> Mesh {
  auto newMesh = Mesh{};

  newMesh.vertices = this->get_vertices(mesh);
  newMesh.indices  = this->get_indices(mesh);
  newMesh.textures = this->get_textures(scene->mMaterials[mesh->mMaterialIndex]);
  newMesh.transform = transform;

  return newMesh;
}

auto ModelLoader::get_vertices(const aiMesh *mesh) -> Mesh::Vertices {
  auto vertices      = Mesh::Vertices{};
  const auto has_uvs = mesh->HasTextureCoords(0);

  vertices.reserve(mesh->mNumVertices);

  for (auto i = size_t{0}; i < mesh->mNumVertices; ++i) {
    auto vertex = Vertex{};

    vertex.position = to_glm(mesh->mVertices[i]);
    vertex.normal   = to_glm(mesh->mNormals[i]);

    if (has_uvs) {
      vertex.uvs       = to_glm(mesh->mTextureCoords[0][i]);
      vertex.tangent   = to_glm(mesh->mTangents[i]);
      vertex.bitangent = to_glm(mesh->mBitangents[i]);
    }

    vertices.push_back(std::move(vertex));
  }

  return vertices;
}

auto ModelLoader::get_indices(const aiMesh *mesh) -> Mesh::Indices {
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

auto ModelLoader::get_material_textures(const aiMaterial *material, Texture::Type type)
    -> Mesh::Textures {
  auto textures = Mesh::Textures{};

  const auto texture_count = material->GetTextureCount(get_assimp_texture_type(type));

  textures.reserve(texture_count);

  for (auto i = 0u; i < texture_count; ++i) {
    auto assimp_path = aiString{};

    material->GetTexture(get_assimp_texture_type(type), i, &assimp_path);
    // FIXME: Export models just with filenames in Blender, remove this.
    auto raw_path = string{assimp_path.C_Str()};
    std::replace(raw_path.begin(), raw_path.end(), '\\', '/');
    const auto file_path = get_texture_path(path{raw_path});

    auto texture      = Texture{};
    texture.type      = type;
    texture.file_path = file_path;
    textures.push_back(texture);
  }

  return textures;
}

auto ModelLoader::get_textures(const aiMaterial *material) -> Mesh::Textures {
  auto textures = Mesh::Textures{};

  const auto diffuse = this->get_material_textures(material, Texture::Type::Diffuse);
  const auto specular = this->get_material_textures(material, Texture::Type::Specular);
  const auto normal = this->get_material_textures(material, Texture::Type::Normal);
  const auto height = this->get_material_textures(material, Texture::Type::Height);

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

auto ModelLoader::get_texture_path(path file_path) const -> path {
  return this->model.file_dir / "textures" / file_path.filename();
}
