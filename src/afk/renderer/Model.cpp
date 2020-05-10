#include "afk/renderer/Model.hpp"

#include <filesystem>

#include "afk/io/ModelLoader.hpp"

using Afk::Model;
using Afk::ModelLoader;

using std::filesystem::path;

Model::Model(const path &_file_path) {
  auto tmp = ModelLoader{}.load(_file_path);

  this->meshes          = std::move(tmp.meshes);
  this->file_path       = std::move(tmp.file_path);
  this->file_dir        = std::move(tmp.file_dir);
  this->nodes           = std::move(tmp.nodes);
  this->node_map        = std::move(tmp.node_map);
  this->root_node_index = std::move(tmp.root_node_index);
  this->animations      = std::move(tmp.animations);
  this->bone_map        = std::move(tmp.bone_map);
  this->bones           = std::move(tmp.bones);
}

