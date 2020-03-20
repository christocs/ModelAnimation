#include "afk/render/ShaderData.hpp"

#include <fstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "afk/io/Log.hpp"
#include "afk/io/Path.hpp"

using Afk::Path;
using Afk::ShaderData;

using namespace std::string_literals;
using std::ifstream;
using std::istreambuf_iterator;
using std::runtime_error;
using std::string;
using std::unordered_map;

static auto getShaderType(const string &extension) -> ShaderData::Type {
  static const auto types = unordered_map<string, ShaderData::Type>{
      {"vert", ShaderData::Type::Vertex},
      {"frag", ShaderData::Type::Fragment},
  };

  return types.at(extension);
}

ShaderData::ShaderData(const string &_path) {
  const auto absPath = Path::getAbsolutePath(_path);

  auto file = ifstream{absPath};

  if (!file) {
    throw runtime_error{"Unable to open shader '"s + absPath + "'"s};
  }

  this->code =
      string{(istreambuf_iterator<char>(file)), istreambuf_iterator<char>()} + '\0';
  this->type = getShaderType(Path::getExtension(_path));
  this->path = _path;
}
