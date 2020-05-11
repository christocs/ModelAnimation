#pragma once

#include <filesystem>

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "afk/renderer/Model.hpp"
#include "afk/renderer/Texture.hpp"

namespace Afk {
  class ModelLoader {
  public:
    Model model = {};

    auto load(const std::filesystem::path &file_path) -> Model;

    constexpr const static double DEFAULT_TICKS_PER_SECOND = 25;

  private:
    auto get_animations(const aiScene *scene) -> void;
    auto process_node(const aiScene *scene, const aiNode *node) -> void;
    auto process_mesh(const aiScene *scene, const aiMesh *mesh, unsigned long node_id) -> Mesh;
    auto get_bones(const aiMesh *mesh) -> void;
    auto get_vertices(const aiMesh *mesh) -> Mesh::Vertices;
    auto get_indices(const aiMesh *mesh) -> Mesh::Indices;
    auto get_textures(const aiMaterial *material) -> Mesh::Textures;
    auto get_material_textures(const aiMaterial *material, Texture::Type type)
        -> Mesh::Textures;
    auto get_texture_path(const std::filesystem::path &file_path) const
        -> std::filesystem::path;
  };
}
