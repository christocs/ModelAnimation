#include "afk/io/ModelLoader.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <glob.h>
#include <iterator>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cpplocate/cpplocate.h>
#include <frozen/unordered_map.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "afk/debug/Assert.hpp"
#include "afk/io/Path.hpp"
#include "afk/renderer/Mesh.hpp"
#include "afk/renderer/Model.hpp"
#include "afk/renderer/Texture.hpp"

using namespace std::string_literals;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using std::string;
using std::unordered_map;
using std::vector;
using std::filesystem::path;

using Afk::ModelLoader;
using Afk::Texture;

constexpr unsigned ASSIMP_OPTIONS =
    aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
    aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_GlobalScale | aiProcess_LimitBoneWeights ;

constexpr auto assimp_texture_types =
    frozen::make_unordered_map<Texture::Type, aiTextureType>({
        {Texture::Type::Diffuse, aiTextureType_DIFFUSE},
        {Texture::Type::Specular, aiTextureType_SPECULAR},
        {Texture::Type::Normal, aiTextureType_NORMALS},
        {Texture::Type::Height, aiTextureType_HEIGHT},
    });

static auto to_glm(aiMatrix4x4t<float> m) -> mat4 {
  return mat4{m.a1, m.b1, m.c1, m.d1,  //
              m.a2, m.b2, m.c2, m.d2,  //
              m.a3, m.b3, m.c3, m.d3,  //
              m.a4, m.b4, m.c4, m.d4}; //
}

static auto to_glm(aiVector3t<float> m) -> vec3 {
  return vec3{m.x, m.y, m.z};
}

auto ModelLoader::load(const path &file_path) -> Model {
  const auto abs_path = Afk::get_absolute_path(file_path);
  auto importer       = Assimp::Importer{};

  this->model.file_path = file_path;
  this->model.file_dir  = file_path.parent_path();

  afk_assert(std::filesystem::exists(abs_path),
             "Model "s + file_path.string() + " doesn't exist"s);

  const auto *scene = importer.ReadFile(abs_path.string(), ASSIMP_OPTIONS);

  afk_assert(scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode,
             "Model load error: "s + importer.GetErrorString());

  this->model.meshes.reserve(scene->mNumMeshes);
  this->model.root_node_index = 0;
  this->model.global_inverse = to_glm(scene->mRootNode->mTransformation.Inverse());
  this->process_node(scene, scene->mRootNode);
  this->get_animations(scene);

  return std::move(this->model);
}

auto ModelLoader::process_node(const aiScene *scene, const aiNode *node) -> void {
  this->model.nodes.push_back(Afk::ModelNode{});
  this->model.nodes.back().transform = to_glm(node->mTransformation);
  this->model.nodes.back().name      = node->mName.C_Str();
  this->model.node_map.insert(std::pair<std::string, unsigned int>(
      node->mName.C_Str(), static_cast<unsigned int>(this->model.nodes.size() - 1)));

  // Process all meshes at this node.
  for (auto i = size_t{0}; i < node->mNumMeshes; ++i) {
    const auto *mesh = scene->mMeshes[node->mMeshes[i]];

    this->model.meshes.push_back(
        this->process_mesh(scene, mesh, this->model.nodes.size() - 1));
    this->model.nodes.back().mesh_ids.push_back(this->model.meshes.size() - 1);
  }

  // Process all child nodes.
  for (auto i = size_t{0}; i < node->mNumChildren; ++i) {
    // add index of child about to be added
    this->model.nodes.back().child_ids.push_back(this->model.nodes.size());
    this->process_node(scene, node->mChildren[i]);
  }
}

auto ModelLoader::process_mesh(const aiScene *scene, const aiMesh *mesh, unsigned long node_id) -> Mesh {
  auto newMesh = Mesh{};

  this->get_bones(mesh);
  newMesh.vertices = this->get_vertices(mesh);
  newMesh.indices  = this->get_indices(mesh);
  newMesh.textures = this->get_textures(scene->mMaterials[mesh->mMaterialIndex]);
  newMesh.node_id = node_id;

  return newMesh;
}

auto ModelLoader::get_bones(const aiMesh *mesh) -> void {
  for (unsigned int i = 0; i < mesh->mNumBones; i++) {
    if (this->model.bone_map.count(mesh->mBones[i]->mName.C_Str()) < 1) {
      this->model.bones.push_back(Bone{to_glm(mesh->mBones[i]->mOffsetMatrix),
                                       to_glm(mesh->mBones[i]->mOffsetMatrix)});
      this->model.bone_map.insert(std::make_pair<std::string, size_t>(
          mesh->mBones[i]->mName.C_Str(), this->model.bones.size() - 1));
    }
  }
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

  for (unsigned int i = 0; i < mesh->mNumBones; i++) {
    for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
      const auto &vertex_id = mesh->mBones[i]->mWeights[j].mVertexId;
      const auto &weight    = mesh->mBones[i]->mWeights[j].mWeight;
      const auto &name      = std::string(mesh->mBones[i]->mName.C_Str());
      afk_assert(vertex_id < vertices.size(), "vertex id out of range");
      const auto bone_id = this->model.bone_map.at(name);
      vertices[vertex_id].add_bone(static_cast<unsigned int>(bone_id), weight);
    }
  }

  return vertices;
}

auto ModelLoader::get_indices(const aiMesh *mesh) -> Mesh::Indices {
  auto indices = Mesh::Indices{};

  indices.reserve(mesh->mNumFaces);

  auto num_indices = 0;
  for (auto i = size_t{0}; i < mesh->mNumFaces; ++i) {
    const auto face = mesh->mFaces[i];

    for (auto j = size_t{0}; j < face.mNumIndices; ++j) {
      indices.push_back(static_cast<Mesh::Index>(face.mIndices[j]));
      ++num_indices;
    }
  }

  return indices;
}

auto ModelLoader::get_animations(const aiScene *scene) -> void {
  for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
    auto animation             = Animation{};
    animation.duration         = scene->mAnimations[i]->mDuration;
    animation.ticks_per_second = scene->mAnimations[i]->mTicksPerSecond > 0
                                     ? scene->mAnimations[i]->mTicksPerSecond
                                     : Afk::ModelLoader::DEFAULT_TICKS_PER_SECOND;

    for (unsigned int j = 0; j < scene->mAnimations[i]->mNumChannels; j++) {
      auto animation_node = Animation::AnimationNode{};

      // process positions
      for (unsigned int k = 0;
           k < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k++) {
        auto &pos_key = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k];
        auto pos = glm::vec3(pos_key.mValue.x, pos_key.mValue.y, pos_key.mValue.z);
        animation_node.position_keys.emplace_back(
            Animation::AnimationNode::PositionKey{pos, pos_key.mTime});
      }
      // process scaling
      for (unsigned int k = 0;
           k < scene->mAnimations[i]->mChannels[j]->mNumScalingKeys; k++) {
        auto &scale_key = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k];
        auto scale =
            glm::vec3(scale_key.mValue.x, scale_key.mValue.y, scale_key.mValue.z);
        animation_node.scaling_keys.emplace_back(
            Animation::AnimationNode::ScaleKey{scale, scale_key.mTime});
      }
      // process rotations
      for (unsigned int k = 0;
           k < scene->mAnimations[i]->mChannels[j]->mNumRotationKeys; k++) {
        auto &rotation_key = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k];
        auto rotation = glm::quat(rotation_key.mValue.w, rotation_key.mValue.x,
                                  rotation_key.mValue.y, rotation_key.mValue.z);
                animation_node.rotation_keys.emplace_back(
                    Animation::AnimationNode::RotationKey{rotation, rotation_key.mTime});
      }

      auto node_id = this->model.node_map.at(
          std::string(scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str()));
      animation.animation_nodes.insert(std::make_pair<unsigned int, Animation::AnimationNode>(
          std::move(node_id), std::move(animation_node)));
    }

    this->model.animations.insert(std::make_pair<std::string, Animation>(
        std::string(scene->mAnimations[i]->mName.C_Str()), std::move(animation)));
  }
}

auto ModelLoader::get_material_textures(const aiMaterial *material, Texture::Type type)
    -> Mesh::Textures {
  auto textures = Mesh::Textures{};

  const auto texture_count = material->GetTextureCount(assimp_texture_types.at(type));

  textures.reserve(texture_count);

  for (auto i = 0u; i < texture_count; ++i) {
    auto assimp_path = aiString{};
    material->GetTexture(assimp_texture_types.at(type), i, &assimp_path);
    const auto file_path = get_texture_path(path{string{assimp_path.C_Str()}});

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

auto ModelLoader::get_texture_path(const path &file_path) const -> path {
  return this->model.file_dir / "textures" / file_path.filename();
}
