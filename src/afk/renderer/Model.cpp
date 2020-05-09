#include "afk/renderer/Model.hpp"

#include <filesystem>

#include "afk/io/ModelLoader.hpp"

using Afk::Model;
using Afk::ModelLoader;

using std::filesystem::path;

Model::Model(const path &_file_path) {
  auto tmp = ModelLoader{}.load(_file_path);

  this->meshes    = std::move(tmp.meshes);
  this->file_path = std::move(tmp.file_path);
  this->file_dir  = std::move(tmp.file_dir);
  this->nodes     = std::move(tmp.nodes);
}

// auto Afk::Model::find_bone_index(const std::string& name, const Bones& bones) -> int
//{
//  int index = -1;
//
//  for(size_t i = 0; i < bones.size(); i++) {
//    if (bones[i].name.compare(name) == 0) {
//      index = static_cast<int>(i);
//      break;
//    }
//  }
//
//  return index;
//}
