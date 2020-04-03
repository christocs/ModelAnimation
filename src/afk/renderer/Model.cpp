#include "afk/renderer/Model.hpp"

#include <string>

#include "afk/io/ModelLoader.hpp"

using std::string;

using Afk::Model;
using Afk::ModelLoader;

Model::Model(const string &_path) {
  auto tmp = ModelLoader{}.load(_path);

  this->meshes = std::move(tmp.meshes);
  this->path   = std::move(tmp.path);
  this->dir    = std::move(tmp.dir);
}
