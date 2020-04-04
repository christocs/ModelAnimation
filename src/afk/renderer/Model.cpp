#include "afk/renderer/Model.hpp"

#include <filesystem>

#include "afk/io/ModelLoader.hpp"

using Afk::Model;
using Afk::ModelLoader;

using std::filesystem::path;

Model::Model(path _file_path) {
  auto tmp = ModelLoader{}.load(_file_path);

  this->meshes    = std::move(tmp.meshes);
  this->file_path = std::move(tmp.file_path);
  this->file_dir  = std::move(tmp.file_dir);
}
