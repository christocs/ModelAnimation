#include "afk/render/ModelData.hpp"

#include <string>

#include "afk/io/ModelLoader.hpp"

using std::string;

using Afk::ModelData;
using Afk::ModelLoader;

ModelData::ModelData(const string &_path) {
  auto tmp = ModelLoader{}.load(_path);

  this->meshes = std::move(tmp.meshes);
  this->path   = std::move(tmp.path);
  this->dir    = std::move(tmp.dir);
}
